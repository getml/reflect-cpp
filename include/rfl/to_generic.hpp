#ifndef RFL_TO_GENERIC_HPP_
#define RFL_TO_GENERIC_HPP_

#include "Generic.hpp"
#include "generic/write.hpp"

namespace rfl {

/// Generates a generic that is equivalent to the struct _t.
template <class... Ps>
Generic to_generic(const auto& _t) {
  return rfl::generic::write<Ps...>(_t);
}

}  // namespace rfl

#endif
