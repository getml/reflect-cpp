#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

// Enum values must be greater than or equal to RFL_ENUM_RANGE_MIN.
// By default, RFL_ENUM_RANGE_MIN is set to -256.
// To change the default minimum range for all enum types, redefine the macro
// RFL_ENUM_RANGE_MIN.
#if !defined(RFL_ENUM_RANGE_MIN)
#define RFL_ENUM_RANGE_MIN -256
#endif

// Enum values must be less than or equal to RFL_ENUM_RANGE_MAX.
// By default, RFL_ENUM_RANGE_MAX is set to 256.
// To change the default maximum range for all enum types, redefine the macro
// RFL_ENUM_RANGE_MAX.
#if !defined(RFL_ENUM_RANGE_MAX)
#define RFL_ENUM_RANGE_MAX 256
#endif

#ifdef ENCHANTUM_MIN_RANGE
#undef ENCHANTUM_MIN_RANGE
#endif
#define ENCHANTUM_MIN_RANGE RFL_ENUM_RANGE_MIN

#ifdef ENCHANTUM_MAX_RANGE
#undef ENCHANTUM_MAX_RANGE
#endif
#define ENCHANTUM_MAX_RANGE RFL_ENUM_RANGE_MAX

#include <concepts>
#include <limits>
#include <type_traits>
#include <utility>

#include "../../Literal.hpp"
#include "../../thirdparty/enchantum/enchantum.hpp"
#include "Names.hpp"
#include "range_defined.hpp"

// https://en.cppreference.com/w/cpp/language/static_cast:

// 8) A value of integer or enumeration type can be converted to any complete
// enumeration type.

// If the underlying type is not fixed, the behavior is undefined if the value
// of expression is out of range (the range is all values possible for the
// smallest bit-field large enough to hold all enumerators of the target
// enumeration). If the underlying type is fixed, the result is the same as
// converting the original value first to the underlying type of the enumeration
// and then to the enumeration type.

// https://en.cppreference.com/w/cpp/language/enum

// enum struct|class name { enumerator = constexpr , enumerator = constexpr ,
// ... } 	(1)
// ...
// 1) declares a scoped enumeration type whose underlying type is int (the
// keywords class and struct are exactly equivalent)
//
// --> These rules taken together imply that if you EITHER fix the type OR you
// use a scoped integer, static_cast<MyEnum>(some_integer_value) will always be
// defined.

template <enchantum::Enum E>
  requires requires(E e) {
    { e | e } -> std::same_as<E>;
  }
constexpr inline bool enchantum::is_bitflag<E> = true;

// Specialize the enchantum EnumTraits further, so rfl::config::enum_range
// works.
namespace enchantum {

template <SignedEnum E>
  requires rfl::internal::enums::range_defined<E>
struct enum_traits<E> {
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = rfl::config::enum_range<E>::min;
  static constexpr auto max = rfl::config::enum_range<E>::max;
};

template <UnsignedEnum E>
  requires rfl::internal::enums::range_defined<E>
struct enum_traits<E> {
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = rfl::config::enum_range<E>::min;
  static constexpr auto max = rfl::config::enum_range<E>::max;
};

template <UnscopedEnum E>
  requires SignedEnum<E> &&
           (!EnumFixedUnderlying<E>) && rfl::internal::enums::range_defined<E>
struct enum_traits<E> {
  static constexpr auto min = rfl::config::enum_range<E>::min;
  static constexpr auto max = rfl::config::enum_range<E>::max;
};

template <UnscopedEnum E>
  requires UnsignedEnum<E> &&
           (!EnumFixedUnderlying<E>) && rfl::internal::enums::range_defined<E>
struct enum_traits<E> {
  static constexpr auto min = rfl::config::enum_range<E>::min;
  static constexpr auto max = rfl::config::enum_range<E>::max;
};

}  // namespace enchantum

namespace rfl::internal::enums {

template <enchantum::Enum EnumType>
consteval auto get_enum_names() {
  return []<std::size_t... Is>(std::index_sequence<Is...>) {
    constexpr auto& entries = enchantum::entries<EnumType>;
    constexpr auto to_str_lit =
        []<std::size_t... Js>(const char* name, std::index_sequence<Js...>) {
          return StringLiteral<sizeof...(Js) + 1>{name[Js]...};
        };
    return Names<EnumType,
                 Literal<to_str_lit(
                     entries[Is].second.data(),
                     std::make_index_sequence<entries[Is].second.size()>{})...>,
                 entries.size(), enchantum::is_bitflag<EnumType>,
                 entries[Is].first...>{};
  }(std::make_index_sequence<enchantum::count<EnumType>>{});
}
}  // namespace rfl::internal::enums

#endif
