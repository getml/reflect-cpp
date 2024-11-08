#ifndef RFL_UBJSON_WRITE_HPP_
#define RFL_UBJSON_WRITE_HPP_

#include <bit>
#include <cstdint>
#include <jsoncons_ext/ubjson/ubjson_encoder.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::ubjson {

/// Returns UBJSON bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  std::vector<uint8_t> buffer;
  jsoncons::ubjson::ubjson_bytes_encoder encoder(buffer);
  const auto writer = Writer(&encoder);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  return std::vector<char>(std::bit_cast<char*>(buffer.data()),
                           std::bit_cast<char*>(buffer.data() + buffer.size()));
}

/// Writes a UBJSON into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::ubjson

#endif
