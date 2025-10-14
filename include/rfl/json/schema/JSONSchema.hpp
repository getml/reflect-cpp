#ifndef RFL_JSON_SCHEMA_JSONSCHEMA_HPP_
#define RFL_JSON_SCHEMA_JSONSCHEMA_HPP_

#include <map>
#include <string>

#include "../../Flatten.hpp"
#include "../../Literal.hpp"
#include "../../Rename.hpp"
#include "Type.hpp"

namespace rfl::json::schema {

template <class T>
struct JSONSchema {
  Rename<"$schema", Literal<"https://json-schema.org/draft/2020-12/schema">>
      schema{};
  rfl::Rename<"$comment", std::optional<std::string>> comment{};
  Flatten<T> root{};
  rfl::Rename<"$defs", std::map<std::string, Type>> definitions{};
};

}  // namespace rfl::json::schema

#endif
