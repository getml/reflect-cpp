module;

#define RFL_JSONCONS_MODULE_INTERFACE
#include "rfl/cbor.hpp"
#undef RFL_JSONCONS_MODULE_INTERFACE

export module rfl.cbor;

export import rfl;

export namespace rfl::cbor {
using ::rfl::cbor::load;
using ::rfl::cbor::read;
using ::rfl::cbor::save;
using ::rfl::cbor::write;
}
