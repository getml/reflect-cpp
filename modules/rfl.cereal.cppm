module;

#include "rfl/cereal.hpp"

export module rfl.cereal;

export import rfl;

export namespace rfl::cereal {
using ::rfl::cereal::load;
using ::rfl::cereal::read;
using ::rfl::cereal::save;
using ::rfl::cereal::write;
}
