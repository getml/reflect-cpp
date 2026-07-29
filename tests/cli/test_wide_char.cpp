#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_wide_char {

struct Config {
  std::string host;
  int port;
  std::optional<double> rate;
  std::optional<bool> verbose;
};

struct ConfigWstring {
  std::wstring host;
  int port;
  std::optional<double> rate;
  std::optional<bool> verbose;
};

struct ShortConfig {
  std::string host;
  rfl::Short<"p", int> port;
  rfl::Short<"v", bool> verbose;
};

struct ShortConfigWstring {
  std::wstring host;
  rfl::Short<"p", int> port;
  rfl::Short<"v", bool> verbose;
};

struct PositionalConfig {
  rfl::Positional<std::string> input_file;
  rfl::Positional<std::string> output_file;
  bool verbose;
};

struct PositionalConfigWstring {
  rfl::Positional<std::wstring> input_file;
  rfl::Positional<std::wstring> output_file;
  bool verbose;
};

TEST(cli, test_read_wide_basic) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080",
                           L"--rate=1.5", L"--verbose"};
  const auto result = rfl::cli::read<Config>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, "localhost");
  EXPECT_EQ(result.value().port, 8080);
  ASSERT_TRUE(result.value().rate);
  EXPECT_DOUBLE_EQ(*result.value().rate, 1.5);
  ASSERT_TRUE(result.value().verbose);
  EXPECT_TRUE(*result.value().verbose);
}

TEST(cli, test_read_wide_empty) {
  const wchar_t* args[] = {L"program"};
  const auto result = rfl::cli::read<Config>(1, const_cast<wchar_t**>(args));
  ASSERT_FALSE(result);
}

TEST(cli, test_read_wide_bool_true) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=1",
                           L"--rate=0", L"--verbose"};
  const auto result = rfl::cli::read<Config>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_TRUE(result.value().verbose);
  EXPECT_TRUE(*result.value().verbose);
}

TEST(cli, test_read_wide_bool_false) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=1",
                           L"--rate=0", L"--verbose=false"};
  const auto result = rfl::cli::read<Config>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_TRUE(result.value().verbose);
  EXPECT_FALSE(*result.value().verbose);
}

TEST(cli, test_read_wide_short_option) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p", L"9090",
                           L"-v"};
  const auto result =
      rfl::cli::read<ShortConfig>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, "localhost");
  EXPECT_EQ(result.value().port(), 9090);
  EXPECT_TRUE(result.value().verbose());
}

TEST(cli, test_read_wide_short_equals) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p=443", L"-v"};
  const auto result =
      rfl::cli::read<ShortConfig>(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 443);
}

TEST(cli, test_read_wide_positional) {
  const wchar_t* args[] = {L"program", L"in.txt", L"out.txt", L"--verbose"};
  const auto result =
      rfl::cli::read<PositionalConfig>(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "in.txt");
  EXPECT_EQ(result.value().output_file(), "out.txt");
  EXPECT_TRUE(result.value().verbose);
}

TEST(cli, test_read_wide_conflict_named_short) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080",
                           L"-p", L"9090"};
  const auto result =
      rfl::cli::read<ShortConfig>(5, const_cast<wchar_t**>(args));
  ASSERT_FALSE(result);
}

TEST(cli, test_read_wide_negative_value) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p", L"-42",
                           L"-v"};
  const auto result =
      rfl::cli::read<ShortConfig>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), -42);
}

TEST(cli, test_read_wide_valid_ascii) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080"};
  const auto result = rfl::cli::read<Config>(3, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, "localhost");
  EXPECT_EQ(result.value().port, 8080);
}

TEST(cli, test_read_wstring_basic) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080",
                            L"--rate=1.5", L"--verbose"};
  const auto result = rfl::cli::read<ConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, L"localhost");
  EXPECT_EQ(result.value().port, 8080);
  ASSERT_TRUE(result.value().rate);
  EXPECT_DOUBLE_EQ(*result.value().rate, 1.5);
  ASSERT_TRUE(result.value().verbose);
  EXPECT_TRUE(*result.value().verbose);
}

TEST(cli, test_read_wstring_empty) {
  const wchar_t* args[] = {L"program"};
  const auto result = rfl::cli::read<ConfigWstring>(1, const_cast<wchar_t**>(args));
  ASSERT_FALSE(result);
}

TEST(cli, test_read_wstring_bool_true) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=1",
                            L"--rate=0", L"--verbose"};
  const auto result = rfl::cli::read<ConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_TRUE(result.value().verbose);
  EXPECT_TRUE(*result.value().verbose);
}

TEST(cli, test_read_wstring_bool_false) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=1",
                            L"--rate=0", L"--verbose=false"};
  const auto result = rfl::cli::read<ConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_TRUE(result.value().verbose);
  EXPECT_FALSE(*result.value().verbose);
}

TEST(cli, test_read_wstring_short_option) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p", L"9090",
                            L"-v"};
  const auto result =
      rfl::cli::read<ShortConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, L"localhost");
  EXPECT_EQ(result.value().port(), 9090);
  EXPECT_TRUE(result.value().verbose());
}

TEST(cli, test_read_wstring_short_equals) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p=443", L"-v"};
  const auto result =
      rfl::cli::read<ShortConfigWstring>(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 443);
}

TEST(cli, test_read_wstring_positional) {
  const wchar_t* args[] = {L"program", L"in.txt", L"out.txt", L"--verbose"};
  const auto result =
      rfl::cli::read<PositionalConfigWstring>(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), L"in.txt");
  EXPECT_EQ(result.value().output_file(), L"out.txt");
  EXPECT_TRUE(result.value().verbose);
}

TEST(cli, test_read_wstring_conflict_named_short) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080",
                            L"-p", L"9090"};
  const auto result =
      rfl::cli::read<ShortConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_FALSE(result);
}

TEST(cli, test_read_wstring_negative_value) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p", L"-42",
                            L"-v"};
  const auto result =
      rfl::cli::read<ShortConfigWstring>(5, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), -42);
}

TEST(cli, test_read_wstring_valid_ascii) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"--port=8080"};
  const auto result = rfl::cli::read<ConfigWstring>(3, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, L"localhost");
  EXPECT_EQ(result.value().port, 8080);
}

TEST(cli, test_parse_argv_wide_basic) {
  const wchar_t* args[] = {L"program", L"--host=localhost", L"-p=8080",
                           L"positional_arg"};
  const auto result = rfl::cli::parse_argv(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  // parse_argv(wchar_t*) converts to UTF-8 and delegates, so keys are
  // std::string
  EXPECT_EQ(result.value().named.at("host"), "localhost");
  EXPECT_EQ(result.value().short_args.at("p"), "8080");
  EXPECT_EQ(result.value().positional[0], "positional_arg");
}

TEST(cli, test_parse_argv_wide_double_dash) {
  const wchar_t* args[] = {L"program", L"--", L"--not-a-flag", L"positional"};
  const auto result = rfl::cli::parse_argv(4, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().positional[0], "--not-a-flag");
  EXPECT_EQ(result.value().positional[1], "positional");
}

TEST(cli, test_parse_argv_wide_empty) {
  const wchar_t* args[] = {L"program"};
  const auto result = rfl::cli::parse_argv(1, const_cast<wchar_t**>(args));
  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value().named.empty());
  EXPECT_TRUE(result.value().positional.empty());
}

}  // namespace test_wide_char
