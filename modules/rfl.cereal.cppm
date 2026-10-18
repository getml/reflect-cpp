module;

#define RFL_CEREAL_MODULE_INTERFACE
#include "rfl/cereal.hpp"
#undef RFL_CEREAL_MODULE_INTERFACE

export module rfl.cereal;

export import rfl;

export namespace rfl::cereal {
using ::rfl::cereal::load;
using ::rfl::cereal::read;
using ::rfl::cereal::save;
using ::rfl::cereal::write;
}
