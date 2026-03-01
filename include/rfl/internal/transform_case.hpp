#ifndef RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_
#define RFL_INTERNAL_TRANSFORMSNAKECASE_HPP_

#include <algorithm>
#include <ranges>

#include "StringLiteral.hpp"

namespace rfl::internal {

constexpr bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }

constexpr char to_upper(char c) { return c >= 'a' && c <= 'z' ? c + ('A' - 'a') : c; }

constexpr char to_lower(char c) { return is_upper(c) ? c - ('A' - 'a') : c; }

/// Transforms the field name from snake_case to camelCase or PascalCase.
template <internal::StringLiteral _name, bool _capitalize>
consteval auto transform_snake_case() {
  constexpr auto src = _name.string_view();
  constexpr auto len = src.size() - std::ranges::count(src, '_');

  auto result = std::array<char, len + 1>{};
  if constexpr (len == 0) return StringLiteral<len + 1>(result);
  result[0] = _capitalize ? to_upper(src[0]) : to_lower(src[0]);
  size_t shift = 1;
  for (size_t i = 1; i < src.size(); ++i) {
    if (src[i] == '_') continue;
    result[shift] = src[i - 1] == '_' ? to_upper(src[i]) : src[i];
    ++shift;
  }
  return StringLiteral<len + 1>(result);
}

/// Transforms the field name from camel case to snake case.
template <internal::StringLiteral _name>
consteval auto transform_camel_case() {
  constexpr auto src = _name.string_view();
  constexpr auto len = src.size() + std::ranges::count_if(src, is_upper);

  auto result = std::array<char, len + 1>{};
  size_t shift = len;
  for (const char c : src | std::views::reverse) {
    result[--shift] = to_lower(c);
    if (is_upper(c)) result[--shift] = '_';
  }

  if constexpr (!src.empty() && is_upper(src[0])) {
    return StringLiteral<len>(result.data() + 1);
  }
  else {
    return StringLiteral<len + 1>(result);
  }
}

/// Transforms the field name from snake_case to kebab-case.
template <internal::StringLiteral _name>
consteval auto transform_snake_to_kebab() {
  constexpr auto src = _name.string_view();

  auto result = std::array<char, src.size() + 1>{};
  size_t shift = 0;
  for (const char c : src) {
    result[shift++] = c == '_' ? '-' : c;
  }
  return StringLiteral<src.size() + 1>(result);
}

}  // namespace rfl::internal

#endif
