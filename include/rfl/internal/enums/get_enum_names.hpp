#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#include <limits>
#include <type_traits>
#include <utility>

#if __has_include(<source_location>)
#include <source_location>
#endif

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

namespace rfl::internal::enums {

template <class T, bool _is_flag>
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
template <class T, bool _is_flag>
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

struct str_view_agg {
  const char* data;
  std::size_t size;
};

template <enchantum::Enum EnumType, bool _is_flag>
consteval auto get_enum_names() {
  // constexpr auto max = get_range_max<EnumType, _is_flag>();
  // constexpr auto min = get_range_min<EnumType, _is_flag>();
  // constexpr auto range_size = max - min + 1;
  //
  // static_assert(range_size > 0,
  //               "enum_range requires a valid range size. Ensure that max is "
  //               "greater than min.");
  //
  // using EmptyNames = Names<EnumType, rfl::Literal<"">, 0, _is_flag>;

  constexpr auto convert = []() {
    constexpr auto a = enchantum::entries<EnumType>;
    std::array<std::pair<EnumType, str_view_agg>, a.size()> b;
    for (std::size_t i = 0; i < a.size(); ++i) {
      b[i].first = a[i].first;
      b[i].second = {a[i].second.data(), a[i].second.size()};
    }
    return b;
  }();

  // return Names<E, Literal<"">, sizeof...(entries), _is_flag,
  // entries.first...>{};

  return [convert]<std::size_t... Is>(std::index_sequence<Is...>) {
    constexpr auto to_str_lit =
        []<std::size_t... Js>(const char* name, std::index_sequence<Js...>) {
          return StringLiteral<sizeof...(Js) + 1>{name[Js]...};
        };
    return Names<EnumType,
                 Literal<to_str_lit(
                     convert[Is].second.data,
                     std::make_index_sequence<convert[Is].second.size>{})...>,
                 convert.size(), _is_flag, convert[Is].first...>{};
  }(std::make_index_sequence<enchantum::count<EnumType>>{});
}
}  // namespace rfl::internal::enums

#endif
