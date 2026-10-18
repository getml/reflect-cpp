module;

#define RFL_JSONCONS_MODULE_INTERFACE
#include "rfl/ubjson.hpp"
#undef RFL_JSONCONS_MODULE_INTERFACE

export module rfl.ubjson;

export import rfl;

export namespace rfl::ubjson {
using ::rfl::ubjson::load;
using ::rfl::ubjson::read;
using ::rfl::ubjson::save;
using ::rfl::ubjson::write;
}
