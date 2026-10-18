module;

#include "rfl/internal/yaml_module_constants.hpp"
#include "rfl/yaml.hpp"

export module rfl.yaml;

export import rfl;

export namespace rfl::yaml {
using ::rfl::yaml::Writer;
using ::rfl::yaml::load;
using ::rfl::yaml::read;
using ::rfl::yaml::save;
using ::rfl::yaml::write;
}
