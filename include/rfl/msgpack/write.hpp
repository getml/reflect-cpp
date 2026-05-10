#ifndef RFL_MSGPACK_WRITE_HPP_
#define RFL_MSGPACK_WRITE_HPP_

#include <msgpack.h>

#include <ostream>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::msgpack {

/// Returns MSGPACK bytes representation of the object.
/// MessagePack is an efficient binary serialization format similar to JSON but more compact.
/// Uses compile-time reflection to serialize a C++ object to MSGPACK format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to MSGPACK
/// @return A vector of chars containing the MSGPACK binary representation
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

/// Writes a MSGPACK representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to MSGPACK and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to MSGPACK
/// @param _stream The output stream to write MSGPACK binary data to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::msgpack

#endif
