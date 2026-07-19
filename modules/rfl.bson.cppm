module;

#include "rfl/bson.hpp"

export module rfl.bson;

export import rfl;

export using ::bson_oid_t;

export namespace rfl::bson {
using ::rfl::bson::load;
using ::rfl::bson::read;
using ::rfl::bson::save;
using ::rfl::bson::write;
}
