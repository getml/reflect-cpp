#ifndef RFL_ATOMIC_SET_ATOMIC_HPP_
#define RFL_ATOMIC_SET_ATOMIC_HPP_

#include <type_traits>

#include "../Result.hpp"
#include "is_atomic.hpp"

namespace rfl::atomic {

template <class T, class U>
Nothing set_atomic(U&& val, T* _t) {
  is_atomic<std::remove_cvref_t<T>>::set(std::forward<U>(val), _t);
  return Nothing{};
}

template <class T, class U>
Nothing set_atomic(U&& val, T& _t) {
  return set_atomic(std::forward<U>(val), &_t);
}

}  // namespace rfl::atomic

#endif
