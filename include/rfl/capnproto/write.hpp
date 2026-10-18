#ifndef RFL_CAPNPROTO_WRITE_HPP_
#define RFL_CAPNPROTO_WRITE_HPP_

#include <ostream>
#include <type_traits>

#include "../SnakeCaseToCamelCase.hpp"
#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Buffers.hpp"
#include "Parser.hpp"
#include "Writer.hpp"
#include "get_root_name.hpp"
#include "to_schema.hpp"

namespace rfl::capnproto {

/// Returns CAPNPROTO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj, const auto& _schema) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
  using ParentType = parsing::Parent<Writer>;
  static_assert(std::is_same<T, U>(),
                "The schema must be compatible with the type to write.");
  const auto root_name = get_root_name<T, Ps...>();
  const auto message = detail::PackedWriter(_schema.value(), root_name);
  const auto writer = message.writer();
  Parser<T, Processors<SnakeCaseToCamelCase, Ps...>>::write(
      writer, _obj, typename ParentType::Root{});
  return message.bytes();
}

/// Returns CAPNPROTO bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  const auto schema = to_schema<T, Ps...>();
  return write<Ps...>(_obj, schema);
}

/// Writes a CAPNPROTO into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

template <class... Ps>
std::ostream& write(const auto& _obj, const auto& _schema,
                    std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj, _schema);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::capnproto

#endif
