#ifndef RFL_INTERNAL_BIND_TO_TUPLE_HPP_
#define RFL_INTERNAL_BIND_TO_TUPLE_HPP_

#ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/bind_to_tuple.hpp"
#else
#include "cpp26/bind_to_tuple.hpp"
#endif

namespace rfl::internal {

template <class T>
auto bind_to_tuple(T& _t) {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::bind_to_tuple(_t);
#else
  return cpp26::bind_to_tuple(_t);
#endif
}

template <class T, typename F>
auto bind_to_tuple(T& _t, const F& _f) {
  auto view = bind_to_tuple(_t);
  return [&]<std::size_t... _is>(std::index_sequence<_is...>) {
    return rfl::make_tuple(_f(rfl::get<_is>(view))...);
  }(std::make_index_sequence<rfl::tuple_size_v<decltype(view)>>());
}

}  // namespace rfl::internal

#endif
