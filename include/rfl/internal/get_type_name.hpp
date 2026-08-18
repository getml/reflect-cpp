#ifndef RFL_INTERNAL_GETTYPENAME_HPP_
#define RFL_INTERNAL_GETTYPENAME_HPP_
// TODO
// #ifndef REFLECTCPP_USE_CPP26_REFLECTION
#include "cpp20/get_type_name.hpp"
// #else
//  TODO
// #endif

namespace rfl::internal {

template <class T>
consteval auto get_type_name() {
  // #ifndef REFLECTCPP_USE_CPP26_REFLECTION
  return cpp20::get_type_name<T>();
  // #else
  //  TODO
  // #endif
}

}  // namespace rfl::internal

#endif
