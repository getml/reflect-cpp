#ifndef RFL_INTERNAL_BIND_TO_TUPLE_HPP_
#define RFL_INTERNAL_BIND_TO_TUPLE_HPP_

#ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/bind_to_tuple.hpp"
#else
// TODO
#endif

namespace rfl::internal {

template <class T>
auto bind_to_tuple(T& _t) {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::bind_to_tuple(_t);
#else
  // TODO
#endif
}

template <class T, typename F>
auto bind_to_tuple(T& _t, const F& _f) {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::bind_to_tuple(_t, _f);
#else
  // TODO
#endif
}

}  // namespace rfl::internal

#endif
