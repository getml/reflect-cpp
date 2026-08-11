#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

// #ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/get_field_names.hpp"
// #else
//  TODO
// #endif

namespace rfl::internal {

template <class T>
auto get_field_names() {
  // #ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::get_field_names<T>();
  // #else
  //  TODO
  // #endif
}

}  // namespace rfl::internal

#endif
