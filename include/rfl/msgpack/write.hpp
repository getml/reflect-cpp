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

namespace rfl::msgpack {

/// Returns msgpack bytes.
template <class T>
std::vector<char> write(const T& _obj) noexcept {
  using ParentType = parsing::Parent<Writer>;
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);
  msgpack_packer pk;
  msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
  auto w = Writer(&pk);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  auto bytes = std::vector<char>(sbuf.data, sbuf.data + sbuf.size);
  msgpack_sbuffer_destroy(&sbuf);
  return bytes;
}

/// Writes a MSGPACK into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) noexcept {
  auto buffer = write(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::msgpack

#endif
