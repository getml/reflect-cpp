#ifndef RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_
#define RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_

#include <algorithm>
#include <ranges>

#include "StringLiteral.hpp"

namespace rfl::internal {

constexpr bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }

constexpr char to_lower(char c) { return is_upper(c) ? c - ('A' - 'a') : c; }

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

/// Transforms the field name from camel case to snake case.
template <internal::StringLiteral _name>
consteval auto transform_camel_case() {
  constexpr auto src = _name.string_view();
  constexpr auto len = src.size() + std::ranges::count_if(src, is_upper);

  auto result = std::array<char, len + 1>{};
  size_t j = len;
  for (const char c : src | std::views::reverse) {
    result[--j] = to_lower(c);
    if (is_upper(c)) result[--j] = '_';
  }
  if constexpr (!src.empty() && is_upper(src[0])) {
    return StringLiteral<len>(result.data() + 1);
  }
  else {
    return StringLiteral<len + 1>(result);
  }
}

/// Transforms the field name from snake_case to kebab-case.
template <internal::StringLiteral _name, size_t _i = 0, char... chars>
consteval auto transform_snake_to_kebab() {
  if constexpr (_i == _name.arr_.size()) {
    return StringLiteral<sizeof...(chars) + 1>(chars...);

  } else if constexpr (_name.arr_[_i] == '\0') {
    return transform_snake_to_kebab<_name, _name.arr_.size(), chars...>();

  } else if constexpr (_name.arr_[_i] == '_') {
    return transform_snake_to_kebab<_name, _i + 1, chars..., '-'>();

  } else {
    return transform_snake_to_kebab<_name, _i + 1, chars..., _name.arr_[_i]>();
  }
}

}  // namespace rfl::internal

#endif
