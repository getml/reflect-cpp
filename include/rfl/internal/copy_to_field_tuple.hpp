#ifndef RFL_INTERNAL_COPY_TO_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_COPY_TO_FIELD_TUPLE_HPP_

#include "move_to_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
auto copy_to_field_tuple(const T& _t) {
  auto t = _t;
  return move_to_field_tuple(std::move(t));
}

}  // namespace internal
}  // namespace rfl

#endif
