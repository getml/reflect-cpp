#pragma once
#include <concepts>
#include <limits>
#include <string_view>
#include <type_traits>

#ifndef ENCHANTUM_ASSERT
  #include <cassert>
// clang-format off
  #define ENCHANTUM_ASSERT(cond, msg, ...) assert(cond && msg)
// clang-format on
#endif

#ifndef ENCHANTUM_THROW
  // additional info such as local variables are here
  #define ENCHANTUM_THROW(exception, ...) throw exception
#endif

#ifndef ENCHANTUM_MAX_RANGE
  #define ENCHANTUM_MAX_RANGE 256
#endif
#ifndef ENCHANTUM_MIN_RANGE
  #define ENCHANTUM_MIN_RANGE (-ENCHANTUM_MAX_RANGE)
#endif

namespace enchantum {

template<typename T>
concept Enum = std::is_enum_v<T>;

template<typename T>
concept SignedEnum = Enum<T> && std::signed_integral<std::underlying_type_t<T>>;

template<typename T>
concept UnsignedEnum = Enum<T> && !SignedEnum<T>;

template<typename T>
concept ScopedEnum = Enum<T> && (!std::is_convertible_v<T, std::underlying_type_t<T>>);

template<typename T>
concept UnscopedEnum = Enum<T> && !ScopedEnum<T>;

template<typename E, typename Underlying>
concept EnumOfUnderlying = Enum<E> && std::same_as<std::underlying_type_t<E>, Underlying>;

template<Enum E>
inline constexpr bool is_bitflag = requires(E e) {
  requires std::same_as<decltype(e & e), bool> || std::same_as<decltype(e & e), E>;
  { ~e } -> std::same_as<E>;
  { e | e } -> std::same_as<E>;
  { e &= e } -> std::same_as<E&>;
  { e |= e } -> std::same_as<E&>;
};

template<typename T>
concept BitFlagEnum = Enum<T> && is_bitflag<T>;

template<typename T>
concept EnumFixedUnderlying = Enum<T> && requires { T{0}; };

template<typename T>
struct enum_traits;

template<SignedEnum E>
struct enum_traits<E> {
private:
  using U = std::underlying_type_t<E>;
  using L = std::numeric_limits<U>;
public:
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = (L::min)() > ENCHANTUM_MIN_RANGE ? (L::min)() : ENCHANTUM_MIN_RANGE;
  static constexpr auto max = (L::max)() < ENCHANTUM_MAX_RANGE ? (L::max)() : ENCHANTUM_MAX_RANGE;
};

template<UnsignedEnum E>
struct enum_traits<E> {
private:
  using T = std::underlying_type_t<E>;
  using L = std::numeric_limits<T>;
public:
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = []() {
    if constexpr (std::is_same_v<T, bool>)
      return false;
    else
      return (ENCHANTUM_MIN_RANGE) < 0 ? 0 : (ENCHANTUM_MIN_RANGE);
  }();
  static constexpr auto max = []() {
    if constexpr (std::is_same_v<T, bool>)
      return true;
    else
      return (L::max)() < (ENCHANTUM_MAX_RANGE) ? (L::max)() : (ENCHANTUM_MAX_RANGE);
  }();
};

} // namespace enchantum