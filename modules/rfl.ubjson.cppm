module;

#include "rfl/ubjson.hpp"

export module rfl.ubjson;

export import rfl;

export namespace rfl::ubjson {
using ::rfl::ubjson::load;
using ::rfl::ubjson::read;
using ::rfl::ubjson::save;
using ::rfl::ubjson::write;
}
