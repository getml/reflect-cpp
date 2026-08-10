#ifndef RFL_INTERNAL_GETTYPENAME_HPP_
#define RFL_INTERNAL_GETTYPENAME_HPP_

#ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/get_type_name.hpp"
#else
// TODO
#endif

namespace rfl::internal {

template <class T>
consteval auto get_type_name() {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::get_type_name<T>();
#else
// TODO
#endif
}

}  // namespace rfl::internal

#endif
