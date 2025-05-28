#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#include <limits>
#include <type_traits>
#include <utility>
#include <concepts>

#include "../../Literal.hpp"
#include "../../thirdparty/enchantum.hpp"
#include "Names.hpp"

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


namespace rfl::internal::enums {

template <class T, bool _is_flag = enchantum::is_bitflag<T>>
consteval auto get_range_min() {
  using U = std::underlying_type_t<T>;
  if constexpr (_is_flag) {
    return 0;
  } else {
    return std::max(static_cast<decltype(range_min<T>::value)>(
                        std::numeric_limits<U>::min()),
                    range_min<T>::value);
  }
}
template <class T, bool _is_flag = enchantum::is_bitflag<T>>
consteval auto get_range_max() {
  using U = std::underlying_type_t<T>;
  if constexpr (_is_flag) {
    if constexpr (std::is_signed_v<T>) {
      return (sizeof(U) * 8 - 2);
    } else {
      return (sizeof(U) * 8 - 1);
    }
  } else {
    return std::min(static_cast<decltype(range_max<T>::value)>(
                        std::numeric_limits<U>::max()),
                    range_max<T>::value);
  }
}

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
                 entries.size(), enchantum::is_bitflag<EnumType>, entries[Is].first...>{};
  }(std::make_index_sequence<enchantum::count<EnumType>>{});
}
}  // namespace rfl::internal::enums

#endif
