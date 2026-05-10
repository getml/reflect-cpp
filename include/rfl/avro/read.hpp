#ifndef RFL_AVRO_READ_HPP_
#define RFL_AVRO_READ_HPP_

#include <avro.h>

#include <istream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/ptr_cast.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "to_schema.hpp"

namespace rfl::avro {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from an Avro value (internal representation).
/// This is typically used when you already have an Avro value structure.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _obj The Avro value to parse
/// @return The deserialized object or an error
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from Avro binary data with an explicit schema.
/// This version uses a pre-defined schema for deserialization.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes Pointer to the Avro binary data
/// @param _size Size of the binary data in bytes
/// @param _schema The Avro schema to use for deserialization
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size,
    const Schema<T>& _schema) noexcept {
  avro_reader_t avro_reader =
      avro_reader_memory(internal::ptr_cast<const char*>(_bytes), _size);
  avro_value_t root;
  int res = avro_generic_value_new(_schema.iface(), &root);
  if (res != 0) {
    avro_value_decref(&root);
    avro_reader_free(avro_reader);
    return error(std::string("Could not read root value: ") + avro_strerror());
  }
  res = avro_value_read(avro_reader, &root);
  if (res) {
    avro_value_decref(&root);
    avro_reader_free(avro_reader);
    return error(std::string("Could not read root value: ") + avro_strerror());
  }
  auto result = read<T, Ps...>(InputVarType{&root});
  avro_value_decref(&root);
  avro_reader_free(avro_reader);
  return result;
}

/// Parses an object from Avro binary data using reflection to infer the schema.
/// This automatically generates the Avro schema from the C++ type.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes Pointer to the Avro binary data
/// @param _size Size of the binary data in bytes
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(const concepts::ByteLike auto* _bytes, const size_t _size) {
  const auto schema = to_schema<std::remove_cvref_t<T>, Ps...>();
  return read<T, Ps...>(_bytes, _size, schema);
}

/// Parses an object from an Avro binary container with an explicit schema.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes A contiguous container (e.g., std::vector, std::string) containing Avro data
/// @param _schema The Avro schema to use for deserialization
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes,
          const Schema<T>& _schema) noexcept {
  return read<T, Ps...>(_bytes.data(), _bytes.size(), _schema);
}

/// Parses an object from an Avro binary container using reflection to infer the schema.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _bytes A contiguous container (e.g., std::vector, std::string) containing Avro data
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from an input stream containing Avro binary data.
/// Reads the entire stream into memory before deserializing.
/// @tparam T The type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _stream The input stream to read from
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::avro

#endif
