#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <rfl/NoFieldNames.hpp>
#include <rfl/boost_serialization.hpp>
#include <rfl/parsing/VectorParser.hpp>
#include <sstream>

#include "../check_numeric_vector_io.hpp"

namespace test_numeric_vector_io {
namespace {
using R = rfl::boost_serialization::Reader<boost::archive::binary_iarchive>;
using W = rfl::boost_serialization::Writer<boost::archive::binary_oarchive>;
using Ps = rfl::Processors<>;

template <class T>
auto legacy_write(const std::vector<T>& input) {
  std::ostringstream stream;
  boost::archive::binary_oarchive archive(
      stream, boost::archive::no_header | boost::archive::no_tracking);
  W writer(&archive);
  rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::write(
      writer, input, typename rfl::parsing::Parent<W>::Root{});
  const auto str = stream.str();
  return std::vector<char>(str.begin(), str.end());
}

template <class T>
auto legacy_read(const std::vector<char>& bytes) {
  std::istringstream stream(std::string(bytes.begin(), bytes.end()));
  boost::archive::binary_iarchive archive(
      stream, boost::archive::no_header | boost::archive::no_tracking);
  return rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::read(
      R{}, typename R::InputVarType{&archive});
}
}  // namespace

TEST(boost_serialization, numeric_vector_wire_compatibility) {
  check_types(
      [](const auto& input) { return rfl::boost_serialization::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::boost_serialization::read<T>(bytes);
      },
      [](const auto& input) { return legacy_write(input); },
      []<class T>(const auto& bytes) { return legacy_read<T>(bytes); });
}

TEST(boost_serialization, numeric_vector_parent_metadata) {
  check_nested(
      [](const auto& input) { return rfl::boost_serialization::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::boost_serialization::read<T>(bytes);
      });
  check_nested(
      [](const auto& input) {
        return rfl::boost_serialization::write<rfl::NoFieldNames>(input);
      },
      []<class T>(const auto& bytes) {
        return rfl::boost_serialization::read<T, rfl::NoFieldNames>(bytes);
      });
}

TEST(boost_serialization, numeric_vector_excessive_length) {
  const auto make_bytes = [] {
    std::ostringstream stream;
    boost::archive::binary_oarchive archive(
        stream, boost::archive::no_header | boost::archive::no_tracking);
    const auto size = std::numeric_limits<std::uint64_t>::max();
    archive << size;
    const auto str = stream.str();
    return std::vector<char>(str.begin(), str.end());
  };
  EXPECT_FALSE(
      rfl::boost_serialization::read<std::vector<double>>(make_bytes()));
}

}  // namespace test_numeric_vector_io

namespace test_numeric_vector_io {

TEST(boost_serialization, numeric_vector_text_archive_fallback) {
  using I = boost::archive::text_iarchive;
  using O = boost::archive::text_oarchive;
  using TextReader = rfl::boost_serialization::Reader<I>;
  using TextWriter = rfl::boost_serialization::Writer<O>;
  const std::vector<double> input{1.25, -2.5, 0.0};
  std::ostringstream output;
  {
    O archive(output);
    rfl::boost_serialization::write<I>(archive, input);
  }
  std::ostringstream legacy;
  {
    O archive(legacy);
    const TextWriter writer(&archive);
    rfl::parsing::
        VectorParser<TextReader, TextWriter, std::vector<double>, Ps>::write(
            writer, input, typename rfl::parsing::Parent<TextWriter>::Root{});
  }
  EXPECT_EQ(output.str(), legacy.str());
  std::istringstream stream(output.str());
  I archive(stream);
  const auto decoded =
      rfl::boost_serialization::read_from_archive<std::vector<double>, I, O>(
          archive);
  ASSERT_TRUE(decoded) << decoded.error().what();
  EXPECT_EQ(*decoded, input);
}

}  // namespace test_numeric_vector_io
