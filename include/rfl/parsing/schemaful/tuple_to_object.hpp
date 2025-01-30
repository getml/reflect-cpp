#ifndef RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_
#define RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_

#include <string>

#include "../schema/Type.hpp"

namespace rfl::parsing::schemaful {

/// Schemaful formats often don't have an explicit tuple representation.
/// This is the required workaround.
schema::Type::Object tuple_to_object(const schema::Type::Tuple& _tup);

}  // namespace rfl::parsing::schemaful

#endif
