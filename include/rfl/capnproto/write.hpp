#ifndef RFL_CAPNPROTO_WRITE_HPP_
#define RFL_CAPNPROTO_WRITE_HPP_

#include <capnp/dynamic.h>
#include <capnp/message.h>
#include <capnp/schema-parser.h>
#include <capnp/schema.h>

#include <bit>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "to_schema.hpp"

namespace rfl::capnproto {

/// Returns CAPNPROTO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj, const auto& _schema) noexcept {
  /*  using T = std::remove_cvref_t<decltype(_obj)>;
    using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
    using ParentType = parsing::Parent<Writer>;
    static_assert(std::is_same<T, U>(),
                  "The schema must be compatible with the type to write.");
    capnproto_value_t root;
    capnproto_generic_value_new(_schema.iface(), &root);
    const auto writer = Writer(&root);
    Parser<T, Processors<Ps...>>::write(writer, _obj,
                                        typename ParentType::Root{});
    size_t size = 0;
    capnproto_value_sizeof(&root, &size);
    std::vector<char> buffer(size);
    capnproto_writer_t capnproto_writer = capnproto_writer_memory(buffer.data(),
    buffer.size()); capnproto_value_write(capnproto_writer, &root);
    capnproto_value_decref(&root);
    capnproto_writer_free(capnproto_writer);
    return buffer;*/

  using T = std::remove_cvref_t<decltype(_obj)>;
  using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
  using ParentType = parsing::Parent<Writer>;
  static_assert(std::is_same<T, U>(),
                "The schema must be compatible with the type to write.");
  capnp::MallocMessageBuilder message;
  auto root =
      message.initRoot<capnp::DynamicStruct>(_schema.value().asStruct());
  const auto writer = Writer(&root);
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  return std::vector<char>();  // TODO
}

/// Returns CAPNPROTO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  const auto schema = to_schema<T, Ps...>();
  return write<Ps...>(_obj, schema);
}

/// Writes a CAPNPROTO into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::capnproto

#endif
