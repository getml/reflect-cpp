#ifndef RFL_CAPNPROTO_TOSCHEMA_HPP_
#define RFL_CAPNPROTO_TOSCHEMA_HPP_

#include <map>
#include <string>
#include <type_traits>

#include "../Literal.hpp"
#include "../Processors.hpp"
#include "../Variant.hpp"
#include "../json.hpp"
#include "../parsing/schema/Type.hpp"
#include "../parsing/schema/ValidationType.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "schema/Type.hpp"

namespace rfl::capnproto {

std::string to_string_representation(
    const parsing::schema::Definition& internal_schema);

/// Returns the Cap'n Proto schema for a class.
template <class T, class... Ps>
Schema<T> to_schema() noexcept {
  const auto internal_schema =
      parsing::schema::make<Reader, Writer, T, Processors<Ps...>>();
  const auto str = to_string_representation(internal_schema);
  return Schema<T>::from_string(str).value();
}
}  // namespace rfl::capnproto

#endif
