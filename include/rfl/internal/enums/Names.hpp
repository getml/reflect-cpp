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
namespace internal {
namespace enums {

template <class EnumType, class LiteralType, size_t N, auto... _enums>
struct Names {
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
      N == 0, Names<EnumType, NewLiteral, 1, _new_enum>,
      Names<EnumType, define_literal_t<LiteralType, NewLiteral>, N + 1,
            _enums..., _new_enum>>;
};

template <class EnumType, size_t N, StringLiteral... _names, auto... _enums>
auto names_to_enumerator_named_tuple(
    Names<EnumType, Literal<_names...>, N, _enums...>) {
  return make_named_tuple(Field<_names, EnumType>{_enums}...);
}

template <class EnumType, size_t N, StringLiteral... _names, auto... _enums>
auto names_to_underlying_enumerator_named_tuple(
    Names<EnumType, Literal<_names...>, N, _enums...>) {
  return make_named_tuple(Field<_names, std::underlying_type_t<EnumType>>{
      static_cast<std::underlying_type_t<EnumType>>(_enums)}...);
}

template <class EnumType, size_t N, StringLiteral... _names, auto... _enums>
constexpr std::array<std::pair<std::string_view, EnumType>, N>
names_to_enumerator_array(Names<EnumType, Literal<_names...>, N, _enums...>) {
  return {
      std::make_pair(LiteralHelper<_names>::field_.string_view(), _enums)...};
}

template <class EnumType, size_t N, StringLiteral... _names, auto... _enums>
constexpr std::array<
    std::pair<std::string_view, std::underlying_type_t<EnumType>>, N>
names_to_underlying_enumerator_array(
    Names<EnumType, Literal<_names...>, N, _enums...>) {
  return {
      std::make_pair(LiteralHelper<_names>::field_.string_view(),
                     static_cast<std::underlying_type_t<EnumType>>(_enums))...};
}

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
