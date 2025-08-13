#pragma once

#include "common.hpp"
#include "details/optional.hpp"
#include "details/string_view.hpp"
#include "entries.hpp"
#include <type_traits>
#include <utility>

namespace enchantum {

template<typename>
inline constexpr bool has_zero_flag = false;

template<BitFlagEnum E>
inline constexpr bool has_zero_flag<E> = []() {
  for (const auto v : values<E>)
    if (static_cast<std::underlying_type_t<E>>(v) == 0)
      return true;
  return false;
}();


template<typename>
inline constexpr bool is_contiguous = false;

template<Enum E>
inline constexpr bool is_contiguous<E> = []() {
  using T = std::underlying_type_t<E>;
  if constexpr (std::is_same_v<T, bool>) {
    return true;
  }
  else {
    constexpr auto& enums = entries<E>;
    for (std::size_t i = 0; i < enums.size() - 1; ++i)
      if (T(enums[i].first) + 1 != T(enums[i + 1].first))
        return false;
    return true;
  }
}();


template<typename E>
concept ContiguousEnum = Enum<E> && is_contiguous<E>;


template<typename>
inline constexpr bool is_contiguous_bitflag = false;

template<BitFlagEnum E>
inline constexpr bool is_contiguous_bitflag<E> = []() {
  constexpr auto& enums = entries<E>;
  using T               = std::underlying_type_t<E>;
  for (auto i = std::size_t{has_zero_flag<E>}; i < enums.size() - 1; ++i)
    if (T(enums[i].first) << 1 != T(enums[i + 1].first))
      return false;
  return true;
}();

template<typename E>
concept ContiguousBitFlagEnum = BitFlagEnum<E> && is_contiguous_bitflag<E>;

template<Enum E>
[[nodiscard]] constexpr bool contains(const E value) noexcept
{
  for (const auto v : values<E>)
    if (v == value)
      return true;
  return false;
}

template<Enum E>
[[nodiscard]] constexpr bool contains(const std::underlying_type_t<E> value) noexcept
{
  return enchantum::contains(static_cast<E>(value));
}


template<Enum E>
[[nodiscard]] constexpr bool contains(const string_view name) noexcept
{
  for (const auto& s : names<E>)
    if (s == name)
      return true;
  return false;
}


template<Enum E, std::predicate<string_view, string_view> BinaryPredicate>
[[nodiscard]] constexpr bool contains(const string_view name, const BinaryPredicate binary_predicate) noexcept
{
  for (const auto& s : names<E>)
    if (binary_predicate(name, s))
      return true;
  return false;
}

template<ContiguousEnum E>
[[nodiscard]] constexpr bool contains(const E value) noexcept
{
  using T = std::underlying_type_t<E>;
  return T(value) <= T(max<E>) && T(value) >= T(min<E>);
}

namespace details {
  template<typename E>
  struct index_to_enum_functor {
    [[nodiscard]] constexpr optional<E> operator()(const std::size_t index) const noexcept
    {
      optional<E> ret;
      if (index < values<E>.size())
        ret.emplace(values<E>[index]);
      return ret;
    }
  };

  struct enum_to_index_functor {
    template<Enum E>
    [[nodiscard]] constexpr optional<std::size_t> operator()(const E e) const noexcept
    {
      if constexpr (ContiguousEnum<E>) {
        using T = std::underlying_type_t<E>;
        if (enchantum::contains(e))
          return optional<std::size_t>(std::size_t(T(e) - T(min<E>)));
      }
      else {
        for (std::size_t i = 0; i < values<E>.size(); ++i)
          if (values<E>[i] == e)
            return optional<std::size_t>(i);
      }
      return optional<std::size_t>();
    }
  };


  template<Enum E>
  struct cast_functor {
    [[nodiscard]] constexpr optional<E> operator()(const std::underlying_type_t<E> value) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      if (!enchantum::contains<E>(value))
        return a;
      a.emplace(static_cast<E>(value));
      return a;
    }

    [[nodiscard]] constexpr optional<E> operator()(const string_view name) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      for (const auto& [e, s] : entries<E>) {
        if (s == name) {
          a.emplace(e);
          return a;
        }
      }
      return a; // nullopt
    }

    template<std::predicate<string_view, string_view> BinaryPred>
    [[nodiscard]] constexpr optional<E> operator()(const string_view name, const BinaryPred binary_predicate) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      for (const auto& [e, s] : entries<E>) {
        if (binary_predicate(name, s)) {
          a.emplace(e);
          return a;
        }
      }
      return a;
    }
  };

} // namespace details

template<Enum E>
inline constexpr details::index_to_enum_functor<E> index_to_enum{};

inline constexpr details::enum_to_index_functor enum_to_index{};

template<Enum E>
inline constexpr details::cast_functor<E> cast{};


namespace details {
  struct to_string_functor {
    template<Enum E>
    [[nodiscard]] constexpr string_view operator()(const E value) const noexcept
    {
      if (const auto i = enchantum::enum_to_index(value))
        return names<E>[*i];
      return string_view();
    }
  };

} // namespace details
inline constexpr details::to_string_functor to_string{};


} // namespace enchantum