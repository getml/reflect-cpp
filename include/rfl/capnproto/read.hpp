#ifndef RFL_CAPNPROTO_READ_HPP_
#define RFL_CAPNPROTO_READ_HPP_

#include <capnp/dynamic.h>
#include <capnp/serialize-packed.h>
#include <kj/io.h>

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
  const auto array_ptr = kj::ArrayPtr<const kj::byte>(
      internal::ptr_cast<const kj::byte*>(_bytes), _size);
  auto input_stream = kj::ArrayInputStream(array_ptr);
  auto message_reader = capnp::PackedMessageReader(input_stream);
  // TODO: Person is hardcoded.
  const auto root_schema = _schema.value().getNested("Person");
  const auto input_var = InputVarType{
      message_reader.getRoot<capnp::DynamicStruct>(root_schema.asStruct())};
  return read<T, Ps...>(input_var);
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