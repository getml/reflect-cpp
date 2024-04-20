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
  CborParser parser;
  CborValue value;
  msgpack_parser_init(reinterpret_cast<const uint8_t*>(_bytes), _size, 0,
                      &parser, &value);
  auto doc = InputVarType{&value};
  auto result = read<T>(doc);
  return result;
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
