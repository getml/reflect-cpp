#ifndef RFL_MSGPACK_WRITE_HPP_
#define RFL_MSGPACK_WRITE_HPP_

#include <msgpack.h>

#include <ostream>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::msgpack {

/// Returns msgpack bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);
  msgpack_packer pk;
  msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
  auto w = Writer(&pk);
  const auto bytes = [&]() -> Result<std::vector<char>> {
    try {
      Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
      return std::vector<char>(sbuf.data, sbuf.data + sbuf.size);
    } catch (const std::exception& e) {
      return error(e.what());
    }
  }();
  msgpack_sbuffer_destroy(&sbuf);
  return bytes.value();
}

/// Writes a MSGPACK into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::msgpack

#endif
