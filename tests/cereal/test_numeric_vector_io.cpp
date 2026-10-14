#include <rfl/NoFieldNames.hpp>
#include <rfl/cereal.hpp>
#include <rfl/parsing/VectorParser.hpp>
#include <sstream>

#include "../check_numeric_vector_io.hpp"

namespace test_numeric_vector_io {
namespace {
using R = rfl::cereal::Reader;
using W = rfl::cereal::Writer;
using Ps = rfl::Processors<>;

template <class T>
auto legacy_write(const std::vector<T>& input) {
  std::ostringstream stream;
  ::cereal::PortableBinaryOutputArchive archive(stream);
  W writer(&archive);
  rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::write(
      writer, input, typename rfl::parsing::Parent<W>::Root{});
  const auto str = stream.str();
  return std::vector<char>(str.begin(), str.end());
}

template <class T>
auto legacy_read(const std::vector<char>& bytes) {
  std::istringstream stream(std::string(bytes.begin(), bytes.end()));
  ::cereal::PortableBinaryInputArchive archive(stream);
  return rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::read(
      R{}, typename R::InputVarType{&archive});
}
}  // namespace

TEST(cereal, numeric_vector_wire_compatibility) {
  check_types(
      [](const auto& input) { return rfl::cereal::write(input); },
      []<class T>(const auto& bytes) { return rfl::cereal::read<T>(bytes); },
      [](const auto& input) { return legacy_write(input); },
      []<class T>(const auto& bytes) { return legacy_read<T>(bytes); });
}

TEST(cereal, numeric_vector_parent_metadata) {
  check_nested(
      [](const auto& input) { return rfl::cereal::write(input); },
      []<class T>(const auto& bytes) { return rfl::cereal::read<T>(bytes); });
  check_nested(
      [](const auto& input) {
        return rfl::cereal::write<rfl::NoFieldNames>(input);
      },
      []<class T>(const auto& bytes) {
        return rfl::cereal::read<T, rfl::NoFieldNames>(bytes);
      });
}

TEST(cereal, numeric_vector_excessive_length) {
  const auto make_bytes = [] {
    std::ostringstream stream;
    ::cereal::PortableBinaryOutputArchive archive(stream);
    const auto size = std::numeric_limits<std::size_t>::max();
    archive(::cereal::make_size_tag(size));
    const auto str = stream.str();
    return std::vector<char>(str.begin(), str.end());
  };
  EXPECT_FALSE(rfl::cereal::read<std::vector<double>>(make_bytes()));
}

}  // namespace test_numeric_vector_io

namespace test_numeric_vector_io {

TEST(cereal, numeric_vector_endianness) {
  using Archive = ::cereal::PortableBinaryOutputArchive;
  for (const auto options :
       {Archive::Options::LittleEndian(), Archive::Options::BigEndian()}) {
    const auto write = [options](const auto& input) {
      std::ostringstream stream;
      Archive archive(stream, options);
      rfl::cereal::write(input, archive);
      const auto str = stream.str();
      return std::vector<char>(str.begin(), str.end());
    };
    const auto old_write = [options]<class T>(const std::vector<T>& input) {
      std::ostringstream stream;
      Archive archive(stream, options);
      W writer(&archive);
      rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::write(
          writer, input, typename rfl::parsing::Parent<W>::Root{});
      const auto str = stream.str();
      return std::vector<char>(str.begin(), str.end());
    };
    check_types(
        write,
        []<class T>(const auto& bytes) { return rfl::cereal::read<T>(bytes); },
        old_write,
        []<class T>(const auto& bytes) { return legacy_read<T>(bytes); });
  }
}

}  // namespace test_numeric_vector_io

namespace test_numeric_vector_io {
TEST(cereal, numeric_vector_nested_arrays) {
  const std::vector<std::vector<double>> input{{}, {1.25, -2.5}, {3.0}};
  const std::vector<std::deque<double>> legacy{{}, {1.25, -2.5}, {3.0}};
  const auto bytes = rfl::cereal::write(input);
  EXPECT_EQ(bytes, rfl::cereal::write(legacy));
  const auto decoded = rfl::cereal::read<std::vector<std::vector<double>>>(
      rfl::cereal::write(legacy));
  ASSERT_TRUE(decoded) << decoded.error().what();
  EXPECT_EQ(*decoded, input);
  const auto old_decoded =
      rfl::cereal::read<std::vector<std::deque<double>>>(bytes);
  ASSERT_TRUE(old_decoded) << old_decoded.error().what();
  EXPECT_EQ(*old_decoded, legacy);
}
}  // namespace test_numeric_vector_io
