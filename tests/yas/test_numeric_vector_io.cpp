#include <rfl/NoFieldNames.hpp>
#include <rfl/parsing/VectorParser.hpp>
#include <rfl/yas.hpp>
#include <sstream>

#include "../check_numeric_vector_io.hpp"

namespace test_numeric_vector_io {
namespace {
using R = rfl::yas::Reader;
using W = rfl::yas::Writer;
using Ps = rfl::Processors<>;

template <class T>
auto legacy_write(const std::vector<T>& input) {
  ::yas::mem_ostream stream;
  W::OArchive archive(stream);
  W writer(&archive);
  rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::write(
      writer, input, typename rfl::parsing::Parent<W>::Root{});
  const auto buffer = stream.get_intrusive_buffer();
  return std::vector<char>(buffer.data, buffer.data + buffer.size);
}

template <class T>
auto legacy_read(const std::vector<char>& bytes) {
  ::yas::mem_istream stream(bytes.data(), bytes.size());
  R::IArchive archive(stream);
  return rfl::parsing::VectorParser<R, W, std::vector<T>, Ps>::read(
      R{}, typename R::InputVarType{&archive});
}
}  // namespace

TEST(yas, numeric_vector_wire_compatibility) {
  check_types(
      [](const auto& input) { return rfl::yas::write(input); },
      []<class T>(const auto& bytes) { return rfl::yas::read<T>(bytes); },
      [](const auto& input) { return legacy_write(input); },
      []<class T>(const auto& bytes) { return legacy_read<T>(bytes); });
}

TEST(yas, numeric_vector_parent_metadata) {
  check_nested(
      [](const auto& input) { return rfl::yas::write(input); },
      []<class T>(const auto& bytes) { return rfl::yas::read<T>(bytes); });
  check_nested(
      [](const auto& input) {
        return rfl::yas::write<rfl::NoFieldNames>(input);
      },
      []<class T>(const auto& bytes) {
        return rfl::yas::read<T, rfl::NoFieldNames>(bytes);
      });
}

TEST(yas, numeric_vector_excessive_length) {
  const auto make_bytes = [] {
    ::yas::mem_ostream stream;
    W::OArchive archive(stream);
    const auto size = std::numeric_limits<std::size_t>::max();
    archive & size;
    const auto buffer = stream.get_intrusive_buffer();
    return std::vector<char>(buffer.data, buffer.data + buffer.size);
  };
  EXPECT_FALSE(rfl::yas::read<std::vector<double>>(make_bytes()));
}

}  // namespace test_numeric_vector_io
