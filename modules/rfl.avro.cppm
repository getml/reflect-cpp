module;

#include "rfl/avro.hpp"

export module rfl.avro;

export import rfl;

export namespace rfl::avro {
using ::rfl::avro::Schema;
using ::rfl::avro::SchemaImpl;
using ::rfl::avro::load;
using ::rfl::avro::read;
using ::rfl::avro::save;
using ::rfl::avro::to_schema;
using ::rfl::avro::write;
}
