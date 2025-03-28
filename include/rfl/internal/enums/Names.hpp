#ifndef RFL_INTERNAL_ENUMS_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_NAMES_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "../../Literal.hpp"
#include "../../define_literal.hpp"
#include "../../make_named_tuple.hpp"
#include "../StringLiteral.hpp"

namespace rfl {

// Enum values must be greater than or equal to RFL_ENUM_RANGE_MIN.
// By default, RFL_ENUM_RANGE_MIN is set to 0.
// To change the default minimum range for all enum types, redefine the macro
// RFL_ENUM_RANGE_MIN.
#if !defined(RFL_ENUM_RANGE_MIN)
#define RFL_ENUM_RANGE_MIN 0
#endif

// Enum values must be less than or equal to RFL_ENUM_RANGE_MAX.
// By default, RFL_ENUM_RANGE_MAX is set to 127.
// To change the default maximum range for all enum types, redefine the macro
// RFL_ENUM_RANGE_MAX.
#if !defined(RFL_ENUM_RANGE_MAX)
#define RFL_ENUM_RANGE_MAX 127
#endif

namespace config {

// To specify a different range for a particular enum type, specialize the
// enum_range template for that enum type.
template <typename T>
struct enum_range {
  static constexpr int min = RFL_ENUM_RANGE_MIN;
  static constexpr int max = RFL_ENUM_RANGE_MAX;
};
}  // namespace config

namespace internal {
namespace enums {

static_assert(RFL_ENUM_RANGE_MAX > RFL_ENUM_RANGE_MIN,
              "RFL_ENUM_RANGE_MAX must be greater than RFL_ENUM_RANGE_MIN.");

template <typename T, typename = void>
struct range_min : std::integral_constant<int, RFL_ENUM_RANGE_MIN> {};

template <typename T>
struct range_min<T, std::void_t<decltype(config::enum_range<T>::min)>>
    : std::integral_constant<decltype(config::enum_range<T>::min),
                             config::enum_range<T>::min> {};

template <typename T, typename = void>
struct range_max : std::integral_constant<int, RFL_ENUM_RANGE_MAX> {};

template <typename T>
struct range_max<T, std::void_t<decltype(config::enum_range<T>::max)>>
    : std::integral_constant<decltype(config::enum_range<T>::max),
                             config::enum_range<T>::max> {};

template <class EnumType, class LiteralType, size_t N, bool _is_flag,
          auto... _enums>struct Names {
  /// Contains a collection of enums as compile-time strings.
  using Literal = LiteralType;

  /// The number of possible values
  constexpr static size_t size = N;

  /// A list of all the possible enums
  constexpr static std::array<EnumType, N> enums_ =
      std::array<EnumType, N>{_enums...};

  static_assert(N == 0 || LiteralType::size() == N,
                "Size of literal and enum do not match.");

  template <class NewLiteral, auto _new_enum>
  using AddOneType = std::conditional_t<
      N == 0, Names<EnumType, NewLiteral, 1, _is_flag, _new_enum>,
      Names<EnumType, define_literal_t<LiteralType, NewLiteral>, N + 1,
            _is_flag, _enums..., _new_enum>>;
};


template <class EnumType, class LiteralType1, size_t N1, bool _is_flag1,
          auto... _enums1, class LiteralType2, size_t N2, auto... _enums2>
consteval auto operator|(
    Names<EnumType, LiteralType1, N1, _is_flag1, _enums1...>,
    Names<EnumType, LiteralType2, N2, _is_flag1, _enums2...>) {
  using CombinedLiteral = define_literal_t<LiteralType1, LiteralType2>;
  return Names<EnumType, CombinedLiteral, N1 + N2, _is_flag1,
              _enums1..., _enums2...>{};
}

template <class EnumType, size_t N, bool _is_flag, StringLiteral... _names,
          auto... _enums>
auto names_to_enumerator_named_tuple(
    Names<EnumType, Literal<_names...>, N, _is_flag, _enums...>) {
  return make_named_tuple(Field<_names, EnumType>{_enums}...);
}

template <class EnumType, size_t N, bool _is_flag, StringLiteral... _names,
          auto... _enums>
auto names_to_underlying_enumerator_named_tuple(
    Names<EnumType, Literal<_names...>, N, _is_flag, _enums...>) {
  return make_named_tuple(Field<_names, std::underlying_type_t<EnumType>>{
      static_cast<std::underlying_type_t<EnumType>>(_enums)}...);
}

template <class EnumType, size_t N, bool _is_flag, StringLiteral... _names,
          auto... _enums>
constexpr std::array<std::pair<std::string_view, EnumType>, N>
names_to_enumerator_array(
    Names<EnumType, Literal<_names...>, N, _is_flag, _enums...>) {
  return {
      std::make_pair(LiteralHelper<_names>::name_.string_view(), _enums)...};
}

template <class EnumType, size_t N, bool _is_flag, StringLiteral... _names,
          auto... _enums>
constexpr std::array<
    std::pair<std::string_view, std::underlying_type_t<EnumType>>, N>
names_to_underlying_enumerator_array(
    Names<EnumType, Literal<_names...>, N, _is_flag, _enums...>) {
  return {
      std::make_pair(LiteralHelper<_names>::name_.string_view(),
                     static_cast<std::underlying_type_t<EnumType>>(_enums))...};
}

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
