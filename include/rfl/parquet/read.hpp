#ifndef RFL_PARQUET_READ_HPP_
#define RFL_PARQUET_READ_HPP_

#include <parquet.h>

#include <istream>
#include <string>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::parquet {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a PARQUET var.
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from PARQUET using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const char* _bytes,
                                              const size_t _size) {
  parquet_zone mempool;
  parquet_zone_init(&mempool, 2048);
  parquet_object deserialized;
  parquet_unpack(_bytes, _size, NULL, &mempool, &deserialized);
  auto r = read<T, Ps...>(deserialized);
  parquet_zone_destroy(&mempool);
  return r;
}

/// Parses an object from PARQUET using reflection.
template <class T, class... Ps>
auto read(const std::vector<char>& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::parquet

#endif
