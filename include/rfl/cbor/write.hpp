#ifndef RFL_CBOR_WRITE_HPP_
#define RFL_CBOR_WRITE_HPP_

#include <cbor.h>

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace cbor {

template <class... Ps>
void write_into_buffer(const auto& _obj, CborEncoder* _encoder,
                       std::vector<char>* _buffer) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  cbor_encoder_init(_encoder, reinterpret_cast<uint8_t*>(_buffer->data()),
                    _buffer->size(), 0);
  const auto writer = Writer(_encoder);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
}

/// Returns CBOR bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  std::vector<char> buffer(4096);
  CborEncoder encoder;
  write_into_buffer<Ps...>(_obj, &encoder, &buffer);
  const auto total_bytes_needed =
      buffer.size() + cbor_encoder_get_extra_bytes_needed(&encoder);
  if (total_bytes_needed != buffer.size()) {
    buffer.resize(total_bytes_needed);
    write_into_buffer<Ps...>(_obj, &encoder, &buffer);
  }
  const auto length = cbor_encoder_get_buffer_size(
      &encoder, reinterpret_cast<uint8_t*>(buffer.data()));
  buffer.resize(length);
  return buffer;
}

/// Writes a CBOR into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace cbor
}  // namespace rfl

#endif
