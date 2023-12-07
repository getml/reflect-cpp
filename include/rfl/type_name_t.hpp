#ifndef RFL_TYPE_NAME_T_HPP_
#define RFL_TYPE_NAME_T_HPP_

#include <type_traits>

#include "rfl/Literal.hpp"
#include "rfl/internal/get_type_name.hpp"

namespace rfl {

/// Returns a rfl::Literal containing the type name of T.
template <class T>
using type_name_t = rfl::Literal<internal::get_type_name<T>()>;

}  // namespace rfl

#endif
