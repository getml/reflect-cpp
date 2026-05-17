module;

#include "rfl/capnproto.hpp"

export module rfl.capnproto;
export import rfl;

export namespace rfl::capnproto {
using ::rfl::capnproto::get_root_name;
using ::rfl::capnproto::InputObjectType;
using ::rfl::capnproto::InputVarType;
using ::rfl::capnproto::is_named_type;
using ::rfl::capnproto::load;
using ::rfl::capnproto::Parser;
using ::rfl::capnproto::read;
using ::rfl::capnproto::Reader;
using ::rfl::capnproto::save;
using ::rfl::capnproto::Schema;
using ::rfl::capnproto::SchemaHolder;
using ::rfl::capnproto::SchemaImpl;
using ::rfl::capnproto::to_schema;
using ::rfl::capnproto::to_string_representation;
using ::rfl::capnproto::write;
using ::rfl::capnproto::Writer;
}  // namespace rfl::capnproto

export namespace rfl::capnproto::schema {
using ::rfl::capnproto::schema::CapnProtoTypes;
using ::rfl::capnproto::schema::Type;
}  // namespace rfl::capnproto::schema
