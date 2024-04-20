#ifndef RFL_MSGPACK_READ_HPP_
#define RFL_MSGPACK_READ_HPP_

#include <msgpack.h>

#include <istream>
#include <string>

#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace msgpack {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a MSGPACK var.
template <class T>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T>::read(r, _obj);
}

/// Parses an object from MSGPACK using reflection.
template <class T>
Result<internal::wrap_in_rfl_array_t<T>> read(const char* _bytes,
                                              const size_t _size) {
  msgpack_zone mempool;
  msgpack_zone_init(&mempool, 2048);
  msgpack_object deserialized;
  msgpack_unpack(_bytes, _size, NULL, &mempool, &deserialized);
  auto r = read<T>(deserialized);
  msgpack_zone_destroy(&mempool);
  return r;
}

/// Parses an object from MSGPACK using reflection.
template <class T>
auto read(const std::vector<char>& _bytes) {
  return read<T>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream.
template <class T>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T>(bytes.data(), bytes.size());
}

}  // namespace msgpack
}  // namespace rfl

#endif
