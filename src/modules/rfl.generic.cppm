module;

#include "rfl/generic/Parser.hpp"
#include "rfl/generic/Reader.hpp"
#include "rfl/generic/Writer.hpp"
#include "rfl/generic/read.hpp"
#include "rfl/generic/write.hpp"

export module rfl.generic;
export import rfl;

export namespace rfl::generic {
using ::rfl::generic::Parser;
using ::rfl::generic::read;
using ::rfl::generic::Reader;
using ::rfl::generic::write;
using ::rfl::generic::Writer;
}  // namespace rfl::generic
