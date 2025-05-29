#ifndef RFL_INTERNAL_ENUMS_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_NAMES_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "../../Literal.hpp"
#include "../../config.hpp"
#include "../../define_literal.hpp"
#include "../../make_named_tuple.hpp"
#include "../../thirdparty/enchantum/enchantum.hpp"
#include "../StringLiteral.hpp"

namespace rfl::internal::enums {

template <class EnumType, class LiteralType, size_t N, bool _is_flag,
          auto... _enums>
struct Names {
  /// Contains a collection of enums as compile-time strings.
  using Literal = LiteralType;

  /// The number of possible values
  constexpr static size_t size = N;

  /// A list of all the possible enums
  constexpr static auto enums_ = std::array<EnumType, N>{_enums...};
};

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

}  // namespace rfl::internal::enums

#endif

