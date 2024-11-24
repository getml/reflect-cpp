#ifndef RFL_AVRO_READ_HPP_
#define RFL_AVRO_READ_HPP_

#include <avro.h>

#include <bit>
#include <istream>
#include <string>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::avro {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a AVRO var.
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from AVRO using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const char* _bytes,
                                              const size_t _size) {
  AvroParser parser;
  InputVarType doc;
  avro_parser_init(std::bit_cast<const uint8_t*>(_bytes), _size, 0, &parser,
                   &doc.val_);
  auto result = read<T, Ps...>(doc);
  return result;
}

/// Parses an object from AVRO using reflection.
template <class T, class... Ps>
auto read(const std::vector<char>& _bytes, const auto& _schema) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::avro

#endif
