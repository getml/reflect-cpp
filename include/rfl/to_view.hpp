#ifndef RFL_TO_VIEW_HPP_
#define RFL_TO_VIEW_HPP_

#include <iostream>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "internal/to_ptr_named_tuple.hpp"

namespace rfl {

template <class T>
auto to_view(T& _t) {
  return internal::to_ptr_named_tuple(_t);
}

}  // namespace rfl

#endif
