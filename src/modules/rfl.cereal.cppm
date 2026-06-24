module;

#include "rfl/cereal.hpp"

export module rfl.cereal;
export import rfl;

export namespace rfl::cereal {
using ::rfl::cereal::InputVarType;
using ::rfl::cereal::load;
using ::rfl::cereal::Parser;
using ::rfl::cereal::read;
using ::rfl::cereal::Reader;
using ::rfl::cereal::save;
using ::rfl::cereal::write;
using ::rfl::cereal::Writer;
}  // namespace rfl::cereal
