#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#include <limits>
#include <source_location>
#include <type_traits>

#include "../../Literal.hpp"
#include "../../define_literal.hpp"
#include "../../internal/remove_namespaces.hpp"
#include "Names.hpp"
#include "is_scoped_enum.hpp"

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
  // Unfortunately, we cannot avoid the use of a compiler-specific macro for
  // Clang on Windows. For all other compilers, function_name works as intended.
#if defined(__clang__) && defined(_MSC_VER)
  const auto func_name = std::string_view{__PRETTY_FUNCTION__};
#else
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
#endif
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

template <class T>
consteval T calc_greatest_power_of_two() {
  if constexpr (std::is_signed_v<T>) {
    return static_cast<T>(1) << (sizeof(T) * 8 - 2);
  } else {
    return static_cast<T>(1) << (sizeof(T) * 8 - 1);
  }
}

template <class T, bool _is_flag>
consteval T get_max() {
  if constexpr (_is_flag) {
    return calc_greatest_power_of_two<T>();
  } else {
    return std::numeric_limits<T>::max() > 127 ? static_cast<T>(127)
                                               : std::numeric_limits<T>::max();
  }
}

template <class T, bool _is_flag, int _i>
consteval T calc_j() {
  if constexpr (_is_flag) {
    return static_cast<T>(1) << _i;
  } else {
    return static_cast<T>(_i);
  }
}

template <class EnumType, class NamesType, auto _max, bool _is_flag, int _i>
consteval auto get_enum_names_impl() {
  using T = std::underlying_type_t<EnumType>;

  constexpr T j = calc_j<T, _is_flag, _i>();

  constexpr auto name = get_enum_name<static_cast<EnumType>(j)>();

  if constexpr (std::get<0>(name.arr_) == '(') {
    if constexpr (j == _max) {
      return NamesType{};
    } else {
      return get_enum_names_impl<EnumType, NamesType, _max, _is_flag, _i + 1>();
    }
  } else {
    using NewNames = typename NamesType::template AddOneType<
        Literal<remove_namespaces<name>()>, static_cast<EnumType>(j)>;

    if constexpr (j == _max) {
      return NewNames{};
    } else {
      return get_enum_names_impl<EnumType, NewNames, _max, _is_flag, _i + 1>();
    }
  }
}

template <class EnumType, bool _is_flag>
consteval auto get_enum_names() {
  static_assert(is_scoped_enum<EnumType>,
                "You must use scoped enums (using class or struct) for the "
                "parsing to work!");

  static_assert(std::is_integral_v<std::underlying_type_t<EnumType>>,
                "The underlying type of any Enum must be integral!");

  constexpr auto max = get_max<std::underlying_type_t<EnumType>, _is_flag>();

  using EmptyNames = Names<EnumType, rfl::Literal<"">, 0>;

  return get_enum_names_impl<EnumType, EmptyNames, max, _is_flag, 0>();
}

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
