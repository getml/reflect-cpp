#ifndef RFL_JSON_TOSCHEMA_HPP_
#define RFL_JSON_TOSCHEMA_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <string>

#include "../Processors.hpp"
#include "../Variant.hpp"
#include "../common.hpp"
#include "../internal/default_if_missing_v.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Writer.hpp"
#include "schema/JSONSchema.hpp"

namespace rfl::json {

template <class T>
struct TypeHelper {};

template <class... Ts>
struct TypeHelper<rfl::Variant<Ts...>> {
  using JSONSchemaType = rfl::Variant<schema::JSONSchema<Ts>...>;
};

RFL_API std::string to_schema_internal_schema(
    const parsing::schema::Definition& internal_schema, const yyjson_write_flag,
    const bool _no_required, const std::string& comment = "");

/// Returns the JSON schema for a class.
template <class T, class... Ps>
std::string to_schema(const yyjson_write_flag _flag = 0,
                      const std::string& comment = "") {
  using P = Processors<Ps...>;
  const auto internal_schema = parsing::schema::make<Reader, Writer, T, P>();
  return to_schema_internal_schema(internal_schema, _flag,
                                   internal::default_if_missing_v<P>, comment);
}
}  // namespace rfl::json

#endif
