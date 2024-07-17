#ifndef RFL_INTERNAL_VARIANT_IS_ALTERNATIVE_TYPE_HPP_
#define RFL_INTERNAL_VARIANT_IS_ALTERNATIVE_TYPE_HPP_

#include <type_traits>

#include "../element_index.hpp"

namespace rfl::internal::variant {

template <class T, class... AlternativeTypes>
static constexpr bool is_alternative_type() {
  return internal::element_index<std::remove_cvref_t<T>,
                                 std::remove_cvref_t<AlternativeTypes>...>() !=
         -1;
}

}  // namespace rfl::internal::variant

#endif
