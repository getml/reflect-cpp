#ifndef RFL_WRAP_IN_RFL_ARRAY_T_
#define RFL_WRAP_IN_RFL_ARRAY_T_

#include <type_traits>

#include "Array.hpp"

namespace rfl {

template <class T>
struct wrap_in_rfl_array {
  using type = T;
};

template <class T>
  requires std::is_array_v<T>
struct wrap_in_rfl_array<T> {
  using type = Array<T>;
};

template <class T>
using wrap_in_rfl_array_t = typename wrap_in_rfl_array<T>::type;

}  // namespace rfl

#endif
