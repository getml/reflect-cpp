#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

#ifndef REFLECTCPP_USE_CPP26_REFLECTION
#include "cpp20/get_field_names.hpp"
#else
#include "cpp26/get_field_names.hpp"
#endif

namespace rfl::internal {

template <class T>
auto get_field_names() {
#ifndef REFLECTCPP_USE_CPP26_REFLECTION
  return cpp20::get_field_names<T>();
#else
  return cpp26::get_field_names<T>();
#endif
}

}  // namespace rfl::internal

#endif
