module;

#include "rfl/cbor.hpp"

export module rfl.cbor;
export import rfl;

export namespace rfl::cbor {
using ::rfl::cbor::InputObjectType;
using ::rfl::cbor::InputVarType;
using ::rfl::cbor::load;
using ::rfl::cbor::Parser;
using ::rfl::cbor::read;
using ::rfl::cbor::Reader;
using ::rfl::cbor::save;
using ::rfl::cbor::write;
using ::rfl::cbor::Writer;
}  // namespace rfl::cbor
