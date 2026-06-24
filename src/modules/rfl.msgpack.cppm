module;

#include "rfl/msgpack.hpp"

export module rfl.msgpack;
export import rfl;

export namespace rfl::msgpack {
using ::rfl::msgpack::InputObjectType;
using ::rfl::msgpack::InputVarType;
using ::rfl::msgpack::load;
using ::rfl::msgpack::Parser;
using ::rfl::msgpack::read;
using ::rfl::msgpack::Reader;
using ::rfl::msgpack::save;
using ::rfl::msgpack::write;
using ::rfl::msgpack::Writer;
}  // namespace rfl::msgpack
