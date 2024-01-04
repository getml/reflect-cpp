#ifndef RFL_FIELD_NAMES_T_HPP_
#define RFL_FIELD_NAMES_T_HPP_

#include <functional>
#include <type_traits>

#include "internal/get_field_names.hpp"

namespace rfl {

/// Returns a rfl::Literal containing the field names of struct T.
template <class T>
using field_names_t = typename std::invoke_result<
    decltype(internal::get_field_names<std::remove_cvref_t<T>>)>::type;

}  // namespace rfl

#endif
