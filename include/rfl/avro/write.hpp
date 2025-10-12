#ifndef RFL_AVRO_WRITE_HPP_
#define RFL_AVRO_WRITE_HPP_

#include <avro.h>

#include <ostream>
#include <type_traits>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"
//#include "Schema.hpp"
#include "Writer.hpp"
#include "to_schema.hpp"

namespace rfl::avro {

/// Returns AVRO bytes.
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
    throw std::runtime_error(std::string(__FUNCTION__) + " error("+ std::to_string(result)+"): "  + avro_strerror());
  }
const auto writer = Writer(&root);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  size_t size = 0;
  result = avro_value_sizeof(&root, &size);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error("+ std::to_string(result)+"): "  + avro_strerror());
  }
  std::vector<char> buffer(size);
  avro_writer_t avro_writer = avro_writer_memory(buffer.data(), buffer.size());
  result = avro_value_write(avro_writer, &root);
  if (result != 0) {
    throw std::runtime_error(std::string(__FUNCTION__) + " error("+ std::to_string(result)+"): "  + avro_strerror());
  }
  avro_value_decref(&root);
  avro_writer_free(avro_writer);
  return buffer;
}

/// Returns AVRO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  const auto schema = to_schema<T, Ps...>();
  return write<Ps...>(_obj, schema);
}

/// Writes a AVRO into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::avro

#endif
