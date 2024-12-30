#ifndef RFL_CAPNPROTO_WRITE_HPP_
#define RFL_CAPNPROTO_WRITE_HPP_

#include <capnp/dynamic.h>
#include <capnp/message.h>
#include <capnp/schema-parser.h>
#include <capnp/schema.h>
#include <capnp/serialize-packed.h>
#include <kj/io.h>

#include <bit>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "../SnakeCaseToCamelCase.hpp"
#include "../internal/ptr_cast.hpp"
#include "../internal/strings/strings.hpp"
#include "../parsing/Parent.hpp"
#include "../parsing/make_type_name.hpp"
#include "Parser.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "to_schema.hpp"

namespace rfl::capnproto {

/// Returns CAPNPROTO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj, const auto& _schema) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
  using ParentType = parsing::Parent<Writer>;
  static_assert(std::is_same<T, U>(),
                "The schema must be compatible with the type to write.");
  const auto root_name = internal::strings::to_pascal_case(
      parsing::make_type_name<std::remove_cvref_t<T>>());
  const auto root_schema = _schema.value().getNested(root_name.c_str());
  capnp::MallocMessageBuilder message_builder;
  auto root =
      message_builder.initRoot<capnp::DynamicStruct>(root_schema.asStruct());
  const auto writer = Writer(&root);
  Parser<T, Processors<SnakeCaseToCamelCase, Ps...>>::write(
      writer, _obj, typename ParentType::Root{});
  kj::VectorOutputStream output_stream;
  capnp::writePackedMessage(output_stream, message_builder);
  auto arr_ptr = output_stream.getArray();
  return std::vector<char>(internal::ptr_cast<char*>(arr_ptr.begin()),
                           internal::ptr_cast<char*>(arr_ptr.end()));
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
