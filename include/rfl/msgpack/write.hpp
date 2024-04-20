#ifndef RFL_MSGPACK_WRITE_HPP_
#define RFL_MSGPACK_WRITE_HPP_

#include <msgpack.h>

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace msgpack {

template <class T>
void write_into_buffer(const T& _obj, CborEncoder* _encoder,
                       std::vector<char>* _buffer) noexcept {
  using ParentType = parsing::Parent<Writer>;
  msgpack_encoder_init(_encoder, reinterpret_cast<uint8_t*>(_buffer->data()),
                       _buffer->size(), 0);
  const auto writer = Writer(_encoder);
  Parser<T>::write(writer, _obj, typename ParentType::Root{});
}

/// Returns MSGPACK bytes.
template <class T>
std::vector<char> write(const T& _obj) noexcept {
  std::vector<char> buffer(4096);
  CborEncoder encoder;
  write_into_buffer(_obj, &encoder, &buffer);
  const auto total_bytes_needed =
      buffer.size() + msgpack_encoder_get_extra_bytes_needed(&encoder);
  if (total_bytes_needed != buffer.size()) {
    buffer.resize(total_bytes_needed);
    write_into_buffer(_obj, &encoder, &buffer);
  }
  const auto length = msgpack_encoder_get_buffer_size(
      &encoder, reinterpret_cast<uint8_t*>(buffer.data()));
  buffer.resize(length);
  return buffer;
}

/// Writes a MSGPACK into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) noexcept {
  auto buffer = write(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace msgpack
}  // namespace rfl

#endif
