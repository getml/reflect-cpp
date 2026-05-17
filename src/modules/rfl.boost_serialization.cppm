module;

#include "rfl/boost_serialization.hpp"

export module rfl.boost_serialization;
export import rfl;

export namespace rfl::boost_serialization {
using ::rfl::boost_serialization::load;
using ::rfl::boost_serialization::MemBuf;
using ::rfl::boost_serialization::Parser;
using ::rfl::boost_serialization::read;
using ::rfl::boost_serialization::read_from_archive;
using ::rfl::boost_serialization::Reader;
using ::rfl::boost_serialization::save;
using ::rfl::boost_serialization::write;
using ::rfl::boost_serialization::Writer;
}  // namespace rfl::boost_serialization
