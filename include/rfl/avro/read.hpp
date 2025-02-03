#ifndef RFL_AVRO_READ_HPP_
#define RFL_AVRO_READ_HPP_

#include <avro.h>

#include <bit>
#include <istream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "to_schema.hpp"

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
Result<internal::wrap_in_rfl_array_t<T>> read(
    const char* _bytes, const size_t _size, const Schema<T>& _schema) noexcept {
  avro_reader_t avro_reader = avro_reader_memory(_bytes, _size);
  avro_value_t root;
  avro_generic_value_new(_schema.iface(), &root);
  auto err = avro_value_read(avro_reader, &root);
  if (err) {
    avro_value_decref(&root);
    avro_reader_free(avro_reader);
    return error(std::string("Could not read root value: ") + avro_strerror());
  }
  auto result = read<T, Ps...>(InputVarType{&root});
  avro_value_decref(&root);
  avro_reader_free(avro_reader);
  return result;
}

/// Parses an object from AVRO using reflection.
template <class T, class... Ps>
auto read(const char* _bytes, const size_t _size) {
  const auto schema = to_schema<std::remove_cvref_t<T>, Ps...>();
  return read<T, Ps...>(_bytes, _size, schema);
}

/// Parses an object from AVRO using reflection.
template <class T, class... Ps>
auto read(const std::vector<char>& _bytes, const Schema<T>& _schema) noexcept {
  return read<T, Ps...>(_bytes.data(), _bytes.size(), _schema);
}

/// Parses an object from AVRO using reflection.
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

}  // namespace rfl::avro

#endif
