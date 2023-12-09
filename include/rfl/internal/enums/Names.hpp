#ifndef RFL_INTERNAL_ENUMS_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_NAMES_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>

#include "rfl/Literal.hpp"

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType, class LiteralType, size_t N>
struct Names {
  /// Contains a collection of enums as compile-time strings.
  using Literal = LiteralType;

  /// The number of possible values
  constexpr static size_t size = N;

  /// A list of all the possible enums
  std::array<EnumType, N> enums_;

  /// The integer values underlying the enum.
  std::array<std::underlying_type_t<EnumType>, N> integers_;

  static_assert(N == 0 || LiteralType::size() == N,
                "Size of literal and enum do not match.");
};

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
