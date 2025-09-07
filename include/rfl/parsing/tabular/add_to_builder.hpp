#ifndef RFL_PARSING_TABULAR_ADD_TO_BUILDER_HPP_
#define RFL_PARSING_TABULAR_ADD_TO_BUILDER_HPP_

#include <type_traits>

#include "../../named_tuple_t.hpp"
#include "ArrowTypes.hpp"

namespace rfl::parsing::tabular {

template <SerializationType _s, class ValueType, class BuilderType>
inline void add_to_builder(const ValueType& _val, BuilderType* _builder) {
  ArrowTypes<std::remove_cvref_t<ValueType>, _s>::add_to_builder(_val,
                                                                 _builder);
}

}  // namespace rfl::parsing::tabular

#endif
