#ifndef FLEXBUF_READ_HPP_
#define FLEXBUF_READ_HPP_

#include <flatbuffers/flexbuffers.h>

#include <istream>
#include <vector>

#include "../Processors.hpp"
//#include "../Result.hpp"
#include "../concepts.hpp"
//#include "../internal/ptr_cast.hpp"
#include "Parser.hpp"

namespace rfl {
namespace flexbuf {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a FlexBuffers var (internal FlexBuffers reference).
/// FlexBuffers is a schema-less binary format from Google's FlatBuffers project.
/// A FlexBuffers var is the internal reference type used by the flexbuffers library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _obj The FlexBuffers reference object to parse from
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from FlexBuffers bytes using reflection.
/// FlexBuffers is a schema-less, self-describing binary format that supports dynamic typing.
/// It's part of Google's FlatBuffers project and is more flexible than regular FlatBuffers.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes Pointer to byte-like data containing FlexBuffers
/// @param _size The size of the byte array
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(const concepts::ByteLike auto* _bytes, const size_t _size) {
  const InputVarType root =
      flexbuffers::GetRoot(internal::ptr_cast<const uint8_t*>(_bytes), _size);
  return read<T, Ps...>(root);
}

/// Parses an object from FlexBuffers using reflection (contiguous container version).
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes A contiguous byte container (e.g., std::vector<uint8_t>, std::string) containing FlexBuffers data
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream containing FlexBuffers data.
/// Reads FlexBuffers binary data from the stream and constructs a C++ object.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing FlexBuffers binary data
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  const auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace flexbuf
}  // namespace rfl

#endif
