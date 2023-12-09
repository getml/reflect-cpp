#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#include <limits>
#include <source_location>
#include <type_traits>

#include "rfl/Literal.hpp"
#include "rfl/define_literal.hpp"
#include "rfl/internal/enums/Names.hpp"
#include "rfl/internal/remove_namespaces.hpp"

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

namespace rfl {
namespace internal {
namespace enums {

template <auto e>
consteval auto get_enum_name_str_view() {
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
#if defined(__clang__)
  const auto split = func_name.substr(0, func_name.size() - 1);
  return split.substr(split.find("e = ") + 4);
#elif defined(__GNUC__)
  const auto split = func_name.substr(0, func_name.size() - 1);
  return split.substr(split.find("e = ") + 4);
#elif defined(_MSC_VER)
  const auto split = func_name.substr(0, func_name.size() - 7);
  return split.substr(split.find("get_enum_name_str_view<") + 23);
#else
  static_assert(false,
                "You are using an unsupported compiler. Please use GCC, Clang "
                "or MSVC or use rfl::Literal.");
#endif
}

template <auto e>
consteval auto get_enum_name() {
  constexpr auto name = get_enum_name_str_view<e>();
  const auto to_str_lit = [&]<auto... Ns>(std::index_sequence<Ns...>) {
    return StringLiteral<sizeof...(Ns) + 1>{name[Ns]...};
  };
  return to_str_lit(std::make_index_sequence<name.size()>{});
}

template <class EnumType>
constexpr auto start_value =
    Names<EnumType, rfl::Literal<"">, 0>{.enums_ = std::array<EnumType, 0>{}};

template <class EnumType, auto _names = start_value<EnumType>, int _i = 0>
consteval auto get_enum_names() {
  static_assert(
      std::is_enum_v<EnumType> &&
          !std::is_convertible_v<EnumType, std::underlying_type_t<EnumType>>,
      "You must use scoped enums (using class or struct) for the "
      "parsing to work!");

  static_assert(std::is_integral_v<std::underlying_type_t<EnumType>>,
                "The underlying type of any Enum must be integral!");

  constexpr auto max =
      std::numeric_limits<std::underlying_type_t<EnumType>>::max();

  if constexpr (_i == 100 || _i > max) {
    return _names;
  } else {
    constexpr auto name = get_enum_name<static_cast<EnumType>(_i)>();
    if constexpr (std::get<0>(name.arr_) == '(') {
      return get_enum_names<EnumType, _names, _i + 1>();
    } else {
      const auto update_enums = [&]<auto... Ns>(std::index_sequence<Ns...>) {
        return std::array<EnumType, sizeof...(Ns) + 1>{
            _names.enums_[Ns]..., static_cast<EnumType>(_i)};
      };

      using NewNames = std::conditional_t<
          decltype(_names)::size == 0,
          Names<EnumType, Literal<remove_namespaces<name>()>, 1>,
          Names<EnumType,
                define_literal_t<typename decltype(_names)::Literal,
                                 Literal<remove_namespaces<name>()>>,
                decltype(_names)::size + 1>>;

      constexpr auto new_names =
          NewNames{.enums_ = update_enums(
                       std::make_index_sequence<decltype(_names)::size>{})};

      return get_enum_names<EnumType, new_names, _i + 1>();
    }
  }
}

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
