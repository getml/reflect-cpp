module;

#include "rfl/toml.hpp"

export module rfl.toml;

export import rfl;

export namespace rfl::toml {
using ::rfl::toml::load;
using ::rfl::toml::read;
using ::rfl::toml::save;
using ::rfl::toml::write;
}
