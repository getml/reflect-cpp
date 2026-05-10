#ifndef RFL_MSGPACK_READ_HPP_
#define RFL_MSGPACK_READ_HPP_

#include <msgpack.h>

#include <istream>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/ptr_cast.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace msgpack {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a MSGPACK var (internal msgpack object representation).
/// A MSGPACK var is the internal representation used by the msgpack-c library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _obj The MSGPACK variant object to parse from
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from MSGPACK bytes using reflection.
/// MessagePack is an efficient binary serialization format similar to JSON but more compact.
/// This function reads MessagePack bytes and constructs a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes Pointer to byte-like data containing MSGPACK
/// @param _size The size of the byte array
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  msgpack_zone mempool;
  msgpack_zone_init(&mempool, 2048);
  msgpack_object deserialized;
  const auto rc = msgpack_unpack(
      internal::ptr_cast<const char*>(_bytes), _size, NULL, &mempool,
      &deserialized);
  if (rc != MSGPACK_UNPACK_SUCCESS && rc != MSGPACK_UNPACK_EXTRA_BYTES) {
    msgpack_zone_destroy(&mempool);
    return Result<internal::wrap_in_rfl_array_t<T>>(
        error("Failed to unpack msgpack data."));
  }
  auto r = read<T, Ps...>(deserialized);
  msgpack_zone_destroy(&mempool);
  return r;
}

/// Parses an object from MSGPACK using reflection (contiguous container version).
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes A contiguous byte container (e.g., std::vector<uint8_t>, std::string) containing MSGPACK data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream containing MSGPACK data.
/// Reads MessagePack binary data from the stream and constructs a C++ object.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing MSGPACK binary data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace msgpack
}  // namespace rfl

#endif
