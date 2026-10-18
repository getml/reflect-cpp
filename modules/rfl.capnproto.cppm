module;

#define RFL_CAPNPROTO_MODULE_INTERFACE
#include "rfl/capnproto.hpp"
#undef RFL_CAPNPROTO_MODULE_INTERFACE

export module rfl.capnproto;

export import rfl;

export namespace rfl::capnproto {
using ::rfl::capnproto::load;
using ::rfl::capnproto::read;
using ::rfl::capnproto::save;
using ::rfl::capnproto::Schema;
using ::rfl::capnproto::to_schema;
using ::rfl::capnproto::write;
}  // namespace rfl::capnproto
