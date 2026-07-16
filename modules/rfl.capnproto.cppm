module;

#include "rfl/capnproto.hpp"

export module rfl.capnproto;

export import rfl;

export namespace rfl::capnproto {
using ::rfl::capnproto::Schema;
using ::rfl::capnproto::load;
using ::rfl::capnproto::read;
using ::rfl::capnproto::save;
using ::rfl::capnproto::to_schema;
using ::rfl::capnproto::write;
}
