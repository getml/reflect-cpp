module;

#include "rfl/cbor.hpp"

export module rfl.cbor;

export import rfl;

export namespace rfl::cbor {
using ::rfl::cbor::load;
using ::rfl::cbor::read;
using ::rfl::cbor::save;
using ::rfl::cbor::write;
}
