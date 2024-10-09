#ifndef RFL_INTERNAL_VARIANT_IS_CONVERTIBLE_TO_HPP_
#define RFL_INTERNAL_VARIANT_IS_CONVERTIBLE_TO_HPP_

#include <type_traits>

#include "../element_index.hpp"

namespace rfl::internal::variant {

template <class T, class... AlternativeTypes>
static constexpr bool is_convertible_to() {
  return std::disjunction_v<std::is_convertible<T, AlternativeTypes>...>;
}

}  // namespace rfl::internal::variant

#endif
