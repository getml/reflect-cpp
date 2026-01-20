#ifndef RFL_ATOMIC_REMOVE_ATOMIC_T_HPP_
#define RFL_ATOMIC_REMOVE_ATOMIC_T_HPP_

#include <type_traits>

#include "is_atomic.hpp"

namespace rfl::atomic {

template <class T>
using remove_atomic_t =
    typename is_atomic<std::remove_cvref_t<T>>::RemoveAtomicT;

}  // namespace rfl::atomic

#endif
