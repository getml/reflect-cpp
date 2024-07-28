#ifndef RFL_INTERNAL_ISEMPTY_HPP_
#define RFL_INTERNAL_ISEMPTY_HPP_

#include <tuple>
#include <type_traits>

#include "../Tuple.hpp"
#include "is_named_tuple.hpp"

namespace rfl::internal {

template <class T>
constexpr bool is_empty() {
  using U = std::remove_cvref_t<std::remove_pointer_t<T>>;
  if constexpr (is_named_tuple_v<U>) {
    return U::size() == 0;
  } else {
    using TupleType = ptr_tuple_t<U>;
    return rfl::tuple_size_v<TupleType> == 0;
  }
}

}  // namespace rfl::internal

#endif
