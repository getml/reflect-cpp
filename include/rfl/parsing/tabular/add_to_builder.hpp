#ifndef RFL_PARSING_TABULAR_ADD_TO_BUILDER_HPP_
#define RFL_PARSING_TABULAR_ADD_TO_BUILDER_HPP_

#include <type_traits>

#include "../../named_tuple_t.hpp"
#include "ArrowTypes.hpp"

namespace rfl::parsing::tabular {

/**
 * @brief Adds a value to an Arrow builder.
 *
 * @tparam _s The serialization type.
 * @tparam ValueType The type of the value.
 * @tparam BuilderType The type of the builder.
 * @param _val The value to add.
 * @param _builder The builder to add to.
 */
template <SerializationType _s, class ValueType, class BuilderType>
inline void add_to_builder(const ValueType& _val, BuilderType* _builder) {
  ArrowTypes<std::remove_cvref_t<ValueType>, _s>::add_to_builder(_val,
                                                                 _builder);
}

}  // namespace rfl::parsing::tabular

#endif
