#ifndef RFL_INTERNAL_ALLFIELDS_HPP_
#define RFL_INTERNAL_ALLFIELDS_HPP_

#include "is_field.hpp"

namespace rfl::internal {

template <class Head, class... Tail>
constexpr bool all_fields_v = (is_field_v<Head> && ... && is_field_v<Tail>);

}  // namespace rfl::internal

#endif
