#ifndef RFL_INTERNAL_PTRCAST_HPP_
#define RFL_INTERNAL_PTRCAST_HPP_

#include <bit>

namespace rfl::internal {

/// Normally, we would use std::launder(reinterpret_cast<...>(...)),
/// but there are weird issues on GCC 12 under certain compiler settings,
/// so we are using this workaround instead.

template <class T1, class T2>
inline T1 ptr_cast(T2* _ptr) {
  return static_cast<T1>(static_cast<void*>(_ptr));
}

template <class T1, class T2>
inline T1 ptr_cast(const T2* _ptr) {
  return static_cast<T1>(static_cast<const void*>(_ptr));
}

}  // namespace rfl::internal

#endif
