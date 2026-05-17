module;

#include "rfl/json.hpp"

export module rfl.json;
export import rfl;

export namespace rfl::json {
using ::rfl::json::InputObjectType;
using ::rfl::json::InputVarType;
using ::rfl::json::load;
using ::rfl::json::Parser;
using ::rfl::json::pretty;
using ::rfl::json::read;
using ::rfl::json::Reader;
using ::rfl::json::save;
using ::rfl::json::to_schema;
using ::rfl::json::to_schema_internal_schema;
using ::rfl::json::TypeHelper;
using ::rfl::json::write;
using ::rfl::json::Writer;
}  // namespace rfl::json

export namespace rfl::json::schema {
using ::rfl::json::schema::JSONSchema;
using ::rfl::json::schema::Type;
}  // namespace rfl::json::schema
