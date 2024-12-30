#ifndef RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_
#define RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_

#include "StringLiteral.hpp"

namespace rfl::internal {

/// Capitalizes a lower-case character.
template <char c>
consteval char to_upper() {
  if constexpr (c >= 'a' && c <= 'z') {
    return c + ('A' - 'a');
  } else {
    return c;
  }
}

template <char c>
consteval char to_lower() {
  if constexpr (c >= 'A' && c <= 'Z') {
    return c - ('A' - 'a');
  } else {
    return c;
  }
}

/// Transforms the field name from snake case to camel case.
template <internal::StringLiteral _name, bool _capitalize, size_t _i = 0,
          char... chars>
consteval auto transform_snake_case() {
  if constexpr (_i == _name.arr_.size()) {
    return StringLiteral<sizeof...(chars) + 1>(chars...);

  } else if constexpr (_name.arr_[_i] == '_') {
    return transform_snake_case<_name, true, _i + 1, chars...>();

  } else if constexpr (_name.arr_[_i] == '\0') {
    return transform_snake_case<_name, false, _name.arr_.size(), chars...>();

  } else if constexpr (_capitalize) {
    return transform_snake_case<_name, false, _i + 1, chars...,
                                to_upper<_name.arr_[_i]>()>();

  } else if constexpr (sizeof...(chars) == 0) {
    return transform_snake_case<_name, false, _i + 1,
                                to_lower<_name.arr_[_i]>()>();

  } else {
    return transform_snake_case<_name, false, _i + 1, chars...,
                                _name.arr_[_i]>();
  }
}
}  // namespace rfl::internal

#endif
