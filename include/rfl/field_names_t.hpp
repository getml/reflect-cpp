#ifndef RFL_FIELD_NAMES_T_HPP_
#define RFL_FIELD_NAMES_T_HPP_

#include <functional>
#include <type_traits>

#include "internal/get_field_names.hpp"

namespace rfl {

/// Returns a rfl::Literal containing the field names of struct T.
/// The literal contains all field names as compile-time strings that can be used
/// for iteration, validation, or other metaprogramming tasks.
/// @tparam T The struct type to extract field names from
template <class T>
using field_names_t = typename std::invoke_result<
    decltype(internal::get_field_names<std::remove_cvref_t<T>>)>::type;

}  // namespace rfl

#endif
