#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#include <limits>
#include <type_traits>
#include <utility>

#if __has_include(<source_location>)
#include <source_location>
#endif

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

namespace rfl::internal::enums {

template <auto e>
consteval auto get_enum_name_str_view() {
#if __cpp_lib_source_location >= 201907L
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
#elif defined(_MSC_VER)
  // Officially, we only support MSVC versions that are modern enough to contain
  // <source_location>, but inofficially, this might work.
  const auto func_name = std::string_view{__FUNCSIG__};
#else
  const auto func_name = std::string_view{__PRETTY_FUNCTION__};
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

template <class T, bool _is_flag>
consteval auto get_range_min() {
  using U = std::underlying_type_t<T>;
  if constexpr (_is_flag) {
    return 0;
  } else {
    return std::max(std::numeric_limits<U>::min(), range_min<T>::value);
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
    return std::min(std::numeric_limits<U>::max(), range_max<T>::value);
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

template <class EnumType, class LiteralType, size_t N, bool _is_flag, int _i,
          auto... _enums>
consteval auto operator|(Names<EnumType, LiteralType, N, _is_flag, _enums...>,
                         std::integral_constant<int, _i>) {
  using NamesType = Names<EnumType, LiteralType, N, _is_flag, _enums...>;
  using T = std::underlying_type_t<EnumType>;
  constexpr T j = calc_j<T, _is_flag, _i>();
  constexpr auto name = get_enum_name<static_cast<EnumType>(j)>();
  if constexpr (std::get<0>(name.arr_) == '(') {
    return NamesType{};
  } else {
    return typename NamesType::template AddOneType<
        Literal<remove_namespaces<name>()>, static_cast<EnumType>(j)>{};
  }
}

template <class EnumType, class LiteralType, size_t N, bool _is_flag, auto... _enums>
struct NamesCombiner {
  template <int... Is>
  static consteval auto combine(std::integer_sequence<int, Is...>) {
    return (Names<EnumType, LiteralType, N, _is_flag, _enums...>{} | ... |
            std::integral_constant<int, Is>());
  }
};

template <int ChunkSize = 256>
struct CombineNames {
  template <class NamesType, int Start, int End>
  static consteval auto apply() {
    if constexpr (End - Start < ChunkSize) {
      return []<int... Is>(std::integer_sequence<int, Is...>) {
        return (NamesType{} | ... | std::integral_constant<int, Start + Is>());
      }(std::make_integer_sequence<int, End - Start + 1>{});
    } else {
      constexpr int Mid = Start + (End - Start) / 2;
      constexpr auto left = CombineNames<ChunkSize>::template apply<NamesType, Start, Mid>();
      constexpr auto right = CombineNames<ChunkSize>::template apply<NamesType, Mid + 1, End>();
      return left | right;
    }
  }
};

template <class EnumType, bool _is_flag>
consteval auto get_enum_names() {
  static_assert(is_scoped_enum<EnumType>,
              "You must use scoped enums (using class or struct) for the parsing to work!");
  static_assert(std::is_integral_v<std::underlying_type_t<EnumType>>,
              "The underlying type of any Enum must be integral!");

  constexpr auto max = get_range_max<EnumType, _is_flag>();
  constexpr auto min = get_range_min<EnumType, _is_flag>();
  constexpr auto range_size = max - min + 1;

  static_assert(range_size > 0,
              "enum_range requires a valid range size. Ensure that max is greater than min.");

  using EmptyNames = Names<EnumType, rfl::Literal<"">, 0, _is_flag>;

  return CombineNames<>::template apply<EmptyNames, min, max>();
}

}  // namespace rfl::internal::enums

#endif
