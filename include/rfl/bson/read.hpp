#ifndef RFL_BSON_READ_HPP_
#define RFL_BSON_READ_HPP_

#include <bson/bson.h>

#include <istream>
#include <string>

#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace bson {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a BSON var.
template <class T>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T>::read(r, _obj);
}

/// Parses an object from flexbuf using reflection.
template <class T>
Result<internal::wrap_in_rfl_array_t<T>> read(char* _bytes,
                                              const size_t _size) {
  Reader::BSONValue value;
  value.val_.value.v_doc.data_len = static_cast<uint32_t>(_size);
  value.val_.value.v_doc.data = reinterpret_cast<uint8_t*>(_bytes);
  value.val_.value_type = BSON_TYPE_DOCUMENT;
  auto doc = InputVarType{&value};
  auto result = read<T>(doc);
  return result;
}

/// Parses an object from BSON using reflection.
template <class T>
auto read(const std::vector<char>& _bytes) {
  auto bytes = _bytes;
  return read<T>(bytes.data(), bytes.size());
}

/// Parses an object from a stream.
template <class T>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T>(bytes.data(), bytes.size());
}

}  // namespace bson
}  // namespace rfl

#endif
