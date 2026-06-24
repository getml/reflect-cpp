module;

#include "rfl/toml.hpp"

export module rfl.toml;
export import rfl;

export namespace rfl::toml {
using ::rfl::toml::InputVarType;
using ::rfl::toml::load;
using ::rfl::toml::Parser;
using ::rfl::toml::read;
using ::rfl::toml::Reader;
using ::rfl::toml::save;
using ::rfl::toml::write;
using ::rfl::toml::Writer;
}  // namespace rfl::toml
