module;

#include "rfl/flexbuf.hpp"

export module rfl.flexbuf;
export import rfl;

export namespace rfl::flexbuf {
using ::rfl::flexbuf::InputVarType;
using ::rfl::flexbuf::load;
using ::rfl::flexbuf::Parser;
using ::rfl::flexbuf::read;
using ::rfl::flexbuf::Reader;
using ::rfl::flexbuf::save;
using ::rfl::flexbuf::to_buffer;
using ::rfl::flexbuf::write;
using ::rfl::flexbuf::Writer;
}  // namespace rfl::flexbuf
