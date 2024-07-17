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

schema::Type type_to_json_schema_type(const parsing::schema::Type& _type);

template <class T>
struct TypeHelper {};

template <class... Ts>
struct TypeHelper<rfl::Variant<Ts...>> {
  using JSONSchemaType = rfl::Variant<schema::JSONSchema<Ts>...>;
};

/// Returns the JSON schema for a class.
template <class T, class... Ps>
std::string to_schema(const yyjson_write_flag _flag = 0) {
  const auto internal_schema =
      parsing::schema::make<Reader, Writer, T, Processors<Ps...>>();
  auto definitions = std::map<std::string, schema::Type>();
  for (const auto& [k, v] : internal_schema.definitions_) {
    definitions[k] = type_to_json_schema_type(v);
  }
  using JSONSchemaType =
      typename TypeHelper<schema::Type::ReflectionType>::JSONSchemaType;
  const auto to_schema = [&](auto&& _root) -> JSONSchemaType {
    using U = std::decay_t<decltype(_root)>;
    return schema::JSONSchema<U>{.root = std::move(_root),
                                 .definitions = definitions};
  };
  auto root = type_to_json_schema_type(internal_schema.root_);
  const auto json_schema = rfl::visit(to_schema, std::move(root.value));
  return write(json_schema, _flag);
}
}  // namespace rfl::json

#endif
