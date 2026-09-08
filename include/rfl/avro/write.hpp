#ifndef RFL_AVRO_WRITE_HPP_
#define RFL_AVRO_WRITE_HPP_

#include <avro.h>

#include <ostream>
#include <stdexcept>
#include <type_traits>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"
#include "to_schema.hpp"

namespace rfl::avro {

/// Serializes an object to Avro binary format with an explicit schema.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @param _schema The Avro schema to use (must match the object type)
/// @return A vector of bytes containing the Avro binary data
template <class... Ps>
std::vector<char> write(const auto& _obj, const auto& _schema) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
  using ParentType = parsing::Parent<Writer>;
  static_assert(std::is_same<T, U>(),
                "The schema must be compatible with the type to write.");
  avro_value_t root;
  int result = avro_generic_value_new(_schema.iface(), &root);
  if (result != 0) {
    avro_value_decref(&root);
    throw std::runtime_error(std::string(__FUNCTION__) + " error(" +
                             std::to_string(result) + "): " + avro_strerror());
  }
  const auto writer = Writer(&root);
  const auto buffer = [&]() -> Result<std::vector<char>> {
    try {
      Parser<T, Processors<Ps...>>::write(writer, _obj,
                                          typename ParentType::Root{});
    } catch (std::exception& e) {
      return error(e.what());
    }
    size_t size = 0;
    result = avro_value_sizeof(&root, &size);
    if (result != 0) {
      return error(std::string(__FUNCTION__) + " error(" +
                   std::to_string(result) + "): " + avro_strerror());
    }
    std::vector<char> buffer(size);
    avro_writer_t avro_writer =
        avro_writer_memory(buffer.data(), buffer.size());
    result = avro_value_write(avro_writer, &root);
    if (result != 0) {
      avro_writer_free(avro_writer);
      return error(std::string(__FUNCTION__) + " error(" +
                   std::to_string(result) + "): " + avro_strerror());
    }
    avro_writer_free(avro_writer);
    return buffer;
  }();
  avro_value_decref(&root);
  return buffer.value();
}

/// Serializes an object to Avro binary format, inferring the schema from the type.
/// This automatically generates the Avro schema from the C++ type.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @return A vector of bytes containing the Avro binary data
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  const auto schema = to_schema<T, Ps...>();
  return write<Ps...>(_obj, schema);
}

/// Writes an object in Avro binary format to an output stream.
/// @tparam Ps Optional processors to apply during serialization
/// @param _obj The object to serialize
/// @param _stream The output stream to write to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::avro

#endif
