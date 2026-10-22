#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <rfl/io/load_bytes.hpp>
#include <rfl/io/load_string.hpp>
#include <rfl/io/save_bytes.hpp>
#include <rfl/io/save_string.hpp>
#include <stdexcept>
#include <string>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>

#include <thread>
#endif

namespace test_file_io {

class FileIO : public testing::Test {
 protected:
  void SetUp() override {
    const auto stamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    for (unsigned i = 0; i < 100; ++i) {
      dir =
          std::filesystem::temp_directory_path() /
          ("reflectcpp-io-" + std::to_string(stamp) + "-" + std::to_string(i));
      if (std::filesystem::create_directory(dir)) return;
    }
    throw std::runtime_error("Cannot create test directory");
  }
  void TearDown() override {
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
  }
  std::string path() const { return (dir / "data").string(); }
  static void write(const std::string& data, std::ostream& out) {
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
  }
  std::filesystem::path dir;
};

TEST_F(FileIO, BinaryRoundTripAndTruncate) {
  std::string data(2 * 1024 * 1024 + 17, '\0');
  for (std::size_t i = 0; i < data.size(); ++i) data[i] = char(i % 256);
  ASSERT_TRUE(rfl::io::save_bytes(path(), data, write));
  const auto result = rfl::io::load_bytes(path());
  ASSERT_TRUE(result);
  EXPECT_EQ(std::string(result->begin(), result->end()), data);
  ASSERT_TRUE(rfl::io::save_bytes(path(), std::string("short"), write));
  EXPECT_EQ(std::filesystem::file_size(path()), 5);
}

TEST_F(FileIO, EmptyFiles) {
  ASSERT_TRUE(rfl::io::save_bytes(path(), std::string{}, write));
  const auto bytes = rfl::io::load_bytes(path());
  ASSERT_TRUE(bytes);
  EXPECT_TRUE(bytes->empty());
  ASSERT_TRUE(rfl::io::save_string(path(), std::string{}, write));
  const auto text = rfl::io::load_string(path());
  ASSERT_TRUE(text);
  EXPECT_TRUE(text->empty());
}

TEST_F(FileIO, TextModeMatchesStreamIterators) {
  std::string data;
  for (int i = 0; i < 20000; ++i) data += "line\r\nnext\n";
  data.append(
      "\0tail\x1a"
      "end",
      9);
  ASSERT_TRUE(rfl::io::save_bytes(path(), data, write));
  std::ifstream reference(path());
  const std::string expected{std::istreambuf_iterator<char>(reference), {}};
  const auto result = rfl::io::load_string(path());
  ASSERT_TRUE(result);
  EXPECT_EQ(*result, expected);
}

TEST_F(FileIO, TextSaveRoundTrip) {
  const std::string data = "first\nsecond\n";
  ASSERT_TRUE(rfl::io::save_string(path(), data, write));
  const auto result = rfl::io::load_string(path());
  ASSERT_TRUE(result);
  EXPECT_EQ(*result, data);
}

TEST_F(FileIO, MissingFilesAndInvalidOutputPaths) {
  EXPECT_FALSE(rfl::io::load_bytes(path()));
  EXPECT_FALSE(rfl::io::load_string(path()));
  EXPECT_FALSE(rfl::io::save_bytes(dir.string(), std::string("x"), write));
  EXPECT_FALSE(rfl::io::save_string(dir.string(), std::string("x"), write));
}

TEST_F(FileIO, ReadFailuresAreErrors) {
  EXPECT_FALSE(rfl::io::load_bytes(dir.string()));
  EXPECT_FALSE(rfl::io::load_string(dir.string()));
}

TEST_F(FileIO, WriteStateFailuresAreErrors) {
  for (const auto state : {std::ios::failbit, std::ios::badbit}) {
    const auto fail = [state](const auto&, std::ostream& out) {
      out.setstate(state);
    };
    EXPECT_FALSE(rfl::io::save_bytes(path(), 0, fail));
    EXPECT_FALSE(rfl::io::save_string(path(), 0, fail));
  }
}

TEST_F(FileIO, CloseFailuresAreErrors) {
  const auto close_early = [](const auto&, std::ofstream& out) { out.close(); };
  EXPECT_FALSE(rfl::io::save_bytes(path(), 0, close_early));
  EXPECT_FALSE(rfl::io::save_string(path(), 0, close_early));
}

TEST_F(FileIO, WriterExceptionsAreErrors) {
  const auto fail = [](const auto&, std::ostream&) {
    throw std::runtime_error("writer failure");
  };
  const auto bytes = rfl::io::save_bytes(path(), 0, fail);
  const auto text = rfl::io::save_string(path(), 0, fail);
  ASSERT_FALSE(bytes);
  ASSERT_FALSE(text);
  EXPECT_EQ(bytes.error().what(), "writer failure");
  EXPECT_EQ(text.error().what(), "writer failure");
}

#if defined(__unix__) || defined(__APPLE__)
TEST_F(FileIO, NonSeekableFiles) {
  ASSERT_EQ(::mkfifo(path().c_str(), 0600), 0);
  const std::string data(192 * 1024 + 7, 'x');
  for (const bool binary : {true, false}) {
    bool written = false;
    std::jthread writer([&] {
      std::ofstream out(path(), std::ios::binary);
      write(data, out);
      out.close();
      written = bool(out);
    });
    if (binary) {
      const auto result = rfl::io::load_bytes(path());
      ASSERT_TRUE(result);
      EXPECT_EQ(std::string(result->begin(), result->end()), data);
    } else {
      const auto result = rfl::io::load_string(path());
      ASSERT_TRUE(result);
      EXPECT_EQ(*result, data);
    }
    writer.join();
    EXPECT_TRUE(written);
  }
}
#endif

#if defined(__linux__)
TEST_F(FileIO, BufferedOutputFailureAtClose) {
  // A small write can remain buffered until close; /dev/full rejects the flush.
  EXPECT_FALSE(rfl::io::save_bytes("/dev/full", std::string("x"), write));
  EXPECT_FALSE(rfl::io::save_string("/dev/full", std::string("x"), write));
}
#endif

}  // namespace test_file_io
