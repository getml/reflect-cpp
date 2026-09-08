#ifndef RFL_INTERNAL_ALLFIELDS_HPP_
#define RFL_INTERNAL_ALLFIELDS_HPP_

#include "is_field.hpp"

namespace rfl::internal {

template <class... Ts>
constexpr bool all_fields_v = (is_field_v<Ts> && ...);

}  // namespace rfl::internal

#endif
