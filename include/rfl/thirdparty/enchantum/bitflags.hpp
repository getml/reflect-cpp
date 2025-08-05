#pragma once
#include "common.hpp"
#include "details/string_view.hpp"
#include "enchantum.hpp"

#ifndef ENCHANTUM_ALIAS_STRING
  #include <string>
#endif

namespace enchantum {
#ifdef ENCHANTUM_ALIAS_STRING
ENCHANTUM_ALIAS_STRING;
#else
using ::std::string;
#endif
} // namespace enchantum


namespace enchantum {


template<BitFlagEnum E>
inline constexpr E value_ors = [] {
  using T = std::underlying_type_t<E>;
  T ret{};
  for (const auto val : values<E>)
    ret |= static_cast<T>(val);
  return static_cast<E>(ret);
}();


template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const std::underlying_type_t<E> value) noexcept
{
  if (value == 0)
    return has_zero_flag<E>;
  using T      = std::underlying_type_t<E>;
  T valid_bits = 0;

  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    const auto v = static_cast<T>(values<E>[i]);
    if ((value & v) == v)
      valid_bits |= v;
  }
  return valid_bits == value;
}

template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const E value) noexcept
{
  return enchantum::contains_bitflag<E>(static_cast<std::underlying_type_t<E>>(value));
}

template<BitFlagEnum E, std::predicate<string_view, string_view> BinaryPred>
[[nodiscard]] constexpr bool contains_bitflag(const string_view s, const char sep, const BinaryPred binary_pred) noexcept
{
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (!enchantum::contains<E>(s.substr(pos, i - pos)), binary_pred)
      return false;
    pos = i + 1;
  }
  return enchantum::contains<E>(s.substr(pos), binary_pred);
}


template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const string_view s, const char sep = '|') noexcept
{
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (!enchantum::contains<E>(s.substr(pos, i - pos)))
      return false;
    pos = i + 1;
  }
  return enchantum::contains<E>(s.substr(pos));
}


template<typename String = string, BitFlagEnum E>
[[nodiscard]] constexpr String to_string_bitflag(const E value, const char sep = '|')
{
  using T = std::underlying_type_t<E>;
  if constexpr (has_zero_flag<E>)
    if (static_cast<T>(value) == 0)
      return String(names<E>[0]);

  String name;
  T      check_value = 0;
  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    const auto& [v, s] = entries<E>[i];
    if (v == (value & v)) {
      if (!name.empty())
        name.append(1, sep);           // append separator if not the first value
      name.append(s.data(), s.size()); // not using operator += since this may not be std::string_view always
      check_value |= static_cast<T>(v);
    }
  }
  if (check_value == static_cast<T>(value))
    return name;
  return String();
}

template<BitFlagEnum E, std::predicate<string_view, string_view> BinaryPred>
[[nodiscard]] constexpr optional<E> cast_bitflag(const string_view s, const char sep, const BinaryPred binary_pred) noexcept
{
  using T = std::underlying_type_t<E>;
  T           check_value{};
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (const auto v = enchantum::cast<E>(s.substr(pos, i - pos), binary_pred))
      check_value |= static_cast<T>(*v);
    else
      return optional<E>();
    pos = i + 1;
  }

  if (const auto v = enchantum::cast<E>(s.substr(pos), binary_pred))
    return optional<E>(static_cast<E>(check_value | static_cast<T>(*v)));
  return optional<E>();
}

template<BitFlagEnum E>
[[nodiscard]] constexpr optional<E> cast_bitflag(const string_view s, const char sep = '|') noexcept
{
  return enchantum::cast_bitflag<E>(s, sep, [](const auto& a, const auto& b) { return a == b; });
}

template<BitFlagEnum E>
[[nodiscard]] constexpr optional<E> cast_bitflag(const E value) noexcept
{
  using T              = std::underlying_type_t<E>;
  const auto raw_value = static_cast<T>(value);

  if constexpr (has_zero_flag<E>)
    if (raw_value == 0)
      return optional<E>(E{});

  T valid_bits{0};
  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    const auto v = static_cast<T>(values<E>[i]);
    if ((raw_value & v) == v)
      valid_bits |= v;
  }
  return valid_bits == raw_value ? optional<E>(value) : optional<E>();
}


} // namespace enchantum