#ifndef RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_
#define RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_

#include <algorithm>

namespace rfl::internal::variant {

template <unsigned long _max_size, class Head, class... Tail>
consteval unsigned long find_max_size() {
  constexpr auto max_size = std::max(_max_size, sizeof(Head));
  if constexpr (sizeof...(Tail) == 0) {
    return max_size;
  } else {
    return find_max_size<max_size, Tail...>();
  }
}

}  // namespace rfl::internal::variant

#endif
