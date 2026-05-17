module;

#include "rfl/avro.hpp"

export module rfl.avro;
export import rfl;

export namespace rfl::avro {
using ::rfl::avro::InputObjectType;
using ::rfl::avro::InputVarType;
using ::rfl::avro::load;
using ::rfl::avro::Parser;
using ::rfl::avro::read;
using ::rfl::avro::Reader;
using ::rfl::avro::save;
using ::rfl::avro::Schema;
using ::rfl::avro::SchemaImpl;
using ::rfl::avro::to_json_representation;
using ::rfl::avro::to_schema;
using ::rfl::avro::write;
using ::rfl::avro::Writer;
}  // namespace rfl::avro

export namespace rfl::avro::schema {
using ::rfl::avro::schema::Type;
}
