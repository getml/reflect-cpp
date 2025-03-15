#ifndef RFL_JSON_TOSCHEMA_HPP_
#define RFL_JSON_TOSCHEMA_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <map>
#include <string>
#include <type_traits>

#include "../Literal.hpp"
#include "../Processors.hpp"
#include "../Variant.hpp"
#include "../parsing/schema/Type.hpp"
#include "../parsing/schema/ValidationType.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Writer.hpp"
#include "schema/JSONSchema.hpp"
#include "schema/Type.hpp"
#include "write.hpp"

namespace rfl::json {

template <class T>
struct TypeHelper {};

template <class... Ts>
struct TypeHelper<rfl::Variant<Ts...>> {
  using JSONSchemaType = rfl::Variant<schema::JSONSchema<Ts>...>;
};

std::string to_schema_internal_schema(
    const parsing::schema::Definition& internal_schema, const yyjson_write_flag,
    const bool _no_required);

/// Returns the JSON schema for a class.
template <class T, class... Ps>
std::string to_schema(const yyjson_write_flag _flag = 0) {
  using P = Processors<Ps...>;
  const auto internal_schema = parsing::schema::make<Reader, Writer, T, P>();
  return to_schema_internal_schema(internal_schema, _flag,
                                   P::default_if_missing_);
}
}  // namespace rfl::json

#endif
