#ifndef FLEXBUF_WRITE_HPP_
#define FLEXBUF_WRITE_HPP_

#include <flatbuffers/flexbuffers.h>

#include <ostream>
#include <vector>

#include "../Processors.hpp"
//#include "../Ref.hpp"
#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace flexbuf {

/// Converts an object to FlexBuffers and returns a byte buffer.
/// FlexBuffers is a schema-less binary format from Google's FlatBuffers project.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to FlexBuffers
/// @return A vector of uint8_t containing the FlexBuffers binary representation
template <class... Ps>
std::vector<uint8_t> to_buffer(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto fbb = Ref<flexbuffers::Builder>::make();
  auto w = Writer(fbb);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
  fbb->Finish();
  return fbb->GetBuffer();
}

/// Writes an object to FlexBuffers format.
/// FlexBuffers is a schema-less, self-describing binary format that supports dynamic typing.
/// Uses compile-time reflection to serialize a C++ object to FlexBuffers format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to FlexBuffers
/// @return A vector of chars containing the FlexBuffers binary representation
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  const auto buffer = to_buffer<Ps...>(_obj);
  const auto data = internal::ptr_cast<const char*>(buffer.data());
  return std::vector<char>(data, data + buffer.size());
}

/// Writes a FlexBuffers representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to FlexBuffers and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to FlexBuffers
/// @param _stream The output stream to write FlexBuffers binary data to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  const auto buffer = to_buffer<Ps...>(_obj);
  const auto data = internal::ptr_cast<const char*>(buffer.data());
  _stream.write(data, buffer.size());
  return _stream;
}

}  // namespace flexbuf
}  // namespace rfl

#endif
