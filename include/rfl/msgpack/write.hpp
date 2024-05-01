#ifndef RFL_MSGPACK_WRITE_HPP_
#define RFL_MSGPACK_WRITE_HPP_

#include <msgpack.h>

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../Processors.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::msgpack {

/// Returns msgpack bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);
  msgpack_packer pk;
  msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
  auto w = Writer(&pk);
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
  auto bytes = std::vector<char>(sbuf.data, sbuf.data + sbuf.size);
  msgpack_sbuffer_destroy(&sbuf);
  return bytes;
}

/// Writes a MSGPACK into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::msgpack

#endif
