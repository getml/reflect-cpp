#ifndef RFL_PARSING_TABULAR_MAKE_ARROW_SCHEMA_HPP_
#define RFL_PARSING_TABULAR_MAKE_ARROW_SCHEMA_HPP_

#include <type_traits>

#include "../../named_tuple_t.hpp"
#include "make_arrow_builders.hpp"

namespace rfl::parsing::tabular {

/**
 * @brief Returns the Arrow schema for type T.
 *
 * @tparam T The type to get the schema for.
 * @tparam _s The serialization type.
 * @return The Arrow schema.
 */
template <class T, SerializationType _s>
inline auto make_arrow_schema() {
  return ArrowBuildersType<named_tuple_t<std::remove_cvref_t<T>>, _s>::schema();
}

}  // namespace rfl::parsing::tabular

#endif
