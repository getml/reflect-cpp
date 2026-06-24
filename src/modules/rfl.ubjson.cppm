module;

#include "rfl/ubjson.hpp"

export module rfl.ubjson;
export import rfl;

export namespace rfl::ubjson {
using ::rfl::ubjson::InputObjectType;
using ::rfl::ubjson::InputVarType;
using ::rfl::ubjson::load;
using ::rfl::ubjson::Parser;
using ::rfl::ubjson::read;
using ::rfl::ubjson::Reader;
using ::rfl::ubjson::save;
using ::rfl::ubjson::write;
using ::rfl::ubjson::Writer;
}  // namespace rfl::ubjson
