#ifndef RFL_SKIP_HPP_
#define RFL_SKIP_HPP_

#include "internal/Skip.hpp"

namespace rfl {

template <class T>
using Skip = internal::Skip<T, true, true>;

template <class T>
using SkipSerialization = internal::Skip<T, true, false>;

template <class T>
using SkipDeserialization = internal::Skip<T, false, true>;

}  // namespace rfl

#endif

