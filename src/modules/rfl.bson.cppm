module;

#include "rfl/bson.hpp"

export module rfl.bson;
export import rfl;

export namespace rfl::bson {
using ::rfl::bson::InputObjectType;
using ::rfl::bson::InputVarType;
using ::rfl::bson::load;
using ::rfl::bson::Parser;
using ::rfl::bson::read;
using ::rfl::bson::Reader;
using ::rfl::bson::save;
using ::rfl::bson::to_buffer;
using ::rfl::bson::write;
using ::rfl::bson::Writer;
}  // namespace rfl::bson
