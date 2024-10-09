#ifndef RFL_FROM_GENERIC_HPP_
#define RFL_FROM_GENERIC_HPP_

#include "Generic.hpp"
#include "generic/read.hpp"

namespace rfl {

/// Generates the struct T from a named tuple.
template <class T, class... Ps>
auto from_generic(const Generic& _g) {
  return rfl::generic::read<T, Ps...>(_g);
}

}  // namespace rfl

#endif
