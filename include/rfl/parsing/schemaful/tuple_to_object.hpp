#ifndef RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_
#define RFL_PARSING_SCHEMAFUL_TUPLETOOBJECT_HPP_

#include "../schema/Type.hpp"
#include "../../common.hpp"

namespace rfl::parsing::schemaful {

/**
 * @brief Converts a tuple schema type to an object schema type.
 *
 * @param _tup The tuple schema type.
 * @return The object schema type.
 */
RFL_API schema::Type::Object tuple_to_object(const schema::Type::Tuple& _tup);

}  // namespace rfl::parsing::schemaful

#endif
