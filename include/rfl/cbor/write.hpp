#ifndef RFL_CBOR_WRITE_HPP_
#define RFL_CBOR_WRITE_HPP_

#include <cstdint>
#include <jsoncons_ext/cbor/cbor_encoder.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::cbor {

/// Returns CBOR bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  std::vector<uint8_t> buffer;
  jsoncons::cbor::cbor_bytes_encoder encoder(buffer);
  const auto writer = Writer(&encoder);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  return std::vector<char>(
      internal::ptr_cast<char*>(buffer.data()),
      internal::ptr_cast<char*>(buffer.data() + buffer.size()));
}

/// Writes a CBOR into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::cbor

#endif
