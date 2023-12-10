#ifndef RFL_INTERNAL_BIND_TO_TUPLE_HPP_
#define RFL_INTERNAL_BIND_TO_TUPLE_HPP_

#include <iostream>
#include <tuple>
#include <type_traits>

#include "rfl/internal/bind_to_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
constexpr auto to_ptr_tuple(T& _t) {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return to_ptr_tuple(*_t);
  } else {
    return bind_to_tuple(_t, [](auto&& x) {
      return std::addressof(std::forward<decltype(x)>(x));
    });
  }
}

}  // namespace internal
}  // namespace rfl

#endif
