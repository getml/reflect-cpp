#ifndef RFL_INTERNAL_TO_PTR_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_NAMED_TUPLE_HPP_

#ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/to_ptr_named_tuple.hpp"
#else
// TODO
#endif

namespace rfl::internal {

template <class T>
auto to_ptr_named_tuple(T&& _t) {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return internal::cpp20::to_ptr_named_tuple(std::forward<T>(_t));
#else
  // TODO
#endif
}

}  // namespace rfl::internal

#endif
