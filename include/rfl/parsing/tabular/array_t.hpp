#ifndef RFL_PARSING_TABULAR_ARRAYT_HPP_
#define RFL_PARSING_TABULAR_ARRAYT_HPP_

#include "ArrowTypes.hpp"

namespace rfl::parsing::tabular {

template <class T, SerializationType _s>
using array_t = typename ArrowTypes<std::remove_cvref_t<T>, _s>::ArrayType;

}  // namespace rfl::parsing::tabular

#endif
