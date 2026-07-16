module;

#include "rfl/msgpack.hpp"

export module rfl.msgpack;

export import rfl;

export namespace rfl::msgpack {
using ::rfl::msgpack::load;
using ::rfl::msgpack::read;
using ::rfl::msgpack::save;
using ::rfl::msgpack::write;
}
