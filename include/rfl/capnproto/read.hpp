#ifndef RFL_CAPNPROTO_READ_HPP_
#define RFL_CAPNPROTO_READ_HPP_

#include <capnproto.h>

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

namespace rfl::capnproto {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a CAPNPROTO var.
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from CAPNPROTO using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const char* _bytes, const size_t _size, const Schema<T>& _schema) noexcept {
  capnproto_reader_t capnproto_reader = capnproto_reader_memory(_bytes, _size);
  capnproto_value_t root;
  capnproto_generic_value_new(_schema.iface(), &root);
  auto err = capnproto_value_read(capnproto_reader, &root);
  if (err) {
    capnproto_value_decref(&root);
    capnproto_reader_free(capnproto_reader);
    return Error(std::string("Could not read root value: ") +
                 capnproto_strerror());
  }
  auto result = read<T, Ps...>(InputVarType{&root});
  capnproto_value_decref(&root);
  capnproto_reader_free(capnproto_reader);
  return result;
}

/// Parses an object from CAPNPROTO using reflection.
template <class T, class... Ps>
auto read(const char* _bytes, const size_t _size) noexcept {
  const auto schema = to_schema<std::remove_cvref_t<T>, Ps...>();
  return read<T, Ps...>(_bytes, _size, schema);
}

/// Parses an object from CAPNPROTO using reflection.
template <class T, class... Ps>
auto read(const std::vector<char>& _bytes, const Schema<T>& _schema) {
  return read<T, Ps...>(_bytes.data(), _bytes.size(), _schema);
}

/// Parses an object from CAPNPROTO using reflection.
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

}  // namespace rfl::capnproto

#endif
