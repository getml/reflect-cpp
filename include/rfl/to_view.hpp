#ifndef RFL_TO_VIEW_HPP_
#define RFL_TO_VIEW_HPP_

#include <iostream>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "internal/to_ptr_named_tuple.hpp"

namespace rfl {

/// Converts a struct to a view (named tuple of pointers to fields).
/// @param _t The struct to convert to a view
/// @return A named tuple containing pointers to the fields of the struct
template <class T>
auto to_view(T& _t) {
  return internal::to_ptr_named_tuple(_t);
}

}  // namespace rfl

#endif
