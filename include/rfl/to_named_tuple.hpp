#ifndef RFL_TO_NAMED_TUPLE_HPP_
#define RFL_TO_NAMED_TUPLE_HPP_

#include <iostream>
#include <tuple>
#include <type_traits>

#include "always_false.hpp"
#include "internal/copy_to_field_tuple.hpp"
#include "internal/has_flatten_fields.hpp"
#include "internal/is_field.hpp"
#include "internal/is_named_tuple.hpp"
#include "internal/move_field_tuple_to_named_tuple.hpp"
#include "internal/move_to_field_tuple.hpp"
#include "make_named_tuple.hpp"

namespace rfl {

/// Generates the named tuple that is equivalent to the struct _t.
/// If _t already is a named tuple, then _t will be returned.
/// All fields of the struct must be an rfl::Field.
auto to_named_tuple(auto&& _t) {
  using T = std::remove_cvref_t<decltype(_t)>;
  if constexpr (internal::is_named_tuple_v<std::remove_cvref_t<T>>) {
    return _t;
  } else if constexpr (internal::is_field_v<std::remove_cvref_t<T>>) {
    return make_named_tuple(std::forward<T>(_t));
  } else if constexpr (std::is_lvalue_reference<T>{}) {
    auto field_tuple = internal::copy_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  } else {
    auto field_tuple = internal::move_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  }
}

/// Generates the named tuple that is equivalent to the struct _t.
/// If _t already is a named tuple, then _t will be returned.
/// All fields of the struct must be an rfl::Field.
template <class ProcessorsType>
auto to_named_tuple(const auto& _t) {
  using T = std::remove_cvref_t<decltype(_t)>;
  if constexpr (internal::is_named_tuple_v<std::remove_cvref_t<T>>) {
    return _t;
  } else if constexpr (internal::is_field_v<std::remove_cvref_t<T>>) {
    return make_named_tuple(_t);
  } else {
    auto field_tuple = internal::copy_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  }
}

}  // namespace rfl

#endif
