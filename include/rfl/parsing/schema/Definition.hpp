#ifndef RFL_PARSING_SCHEMA_DEFINITION_HPP_
#define RFL_PARSING_SCHEMA_DEFINITION_HPP_

#include <map>
#include <string>

#include "Type.hpp"

namespace rfl::parsing::schema {

struct Definition {
  /// Contains the root element of the schema definition.
  Type root_;

  /// Contains the definitions to be referenced by Type::Reference.
  std::map<std::string, Type> definitions_;
};

}  // namespace rfl::parsing::schema

#endif
