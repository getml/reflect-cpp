#ifndef RFL_INTERNAL_ISEMPTY_HPP_
#define RFL_INTERNAL_ISEMPTY_HPP_

#include <tuple>
#include <type_traits>

#include "is_named_tuple.hpp"

namespace rfl::internal {

template <class T>
constexpr bool is_empty() {
  if constexpr (is_named_tuple_v<T>) {
    return std::remove_cvref_t<std::remove_pointer_t<T>>::size() == 0;
  } else {
    using TupleType = ptr_tuple_t<T>;
    return std::tuple_size_v<TupleType> == 0;
  }
}

}  // namespace rfl::internal

#endif
