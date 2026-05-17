module;

#include "rfl/yaml.hpp"

export module rfl.yaml;
export import rfl;

export namespace rfl::yaml {
using ::rfl::yaml::InputVarType;
using ::rfl::yaml::load;
using ::rfl::yaml::Parser;
using ::rfl::yaml::read;
using ::rfl::yaml::Reader;
using ::rfl::yaml::save;
using ::rfl::yaml::write;
using ::rfl::yaml::Writer;
}  // namespace rfl::yaml
