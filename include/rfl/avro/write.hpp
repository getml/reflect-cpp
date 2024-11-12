#ifndef RFL_AVRO_WRITE_HPP_
#define RFL_AVRO_WRITE_HPP_

#include <avro.h>

#include <bit>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Schema.hpp"

namespace rfl::avro {

/// Returns AVRO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj, const Schema& _schema) noexcept {
  std::vector<char> buffer(4096);
  avro_value_t root;
  avro_generic_value_new(_schema.iface(), &root);
  const auto writer = Writer(&root);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  avro_writer_t avro_writer;
  avro_writer_memory(buffer.data(), buffer.size());
  // TODO: Handle cases in which the buffer isn't large enough.
  avro_value_write(avro_writer, &root);
  const auto len = avro_writer_tell(writer);
  avro_value_decref(&root);
  buffer.resize(len);
  return buffer;
}

/// Writes a AVRO into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  // TODO
  /*auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());*/
  return _stream;
}

}  // namespace rfl::avro

#endif
