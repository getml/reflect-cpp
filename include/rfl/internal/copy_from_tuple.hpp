#ifndef RFL_COPY_FROM_TUPLE_HPP_
#define RFL_COPY_FROM_TUPLE_HPP_

#include "move_from_tuple.hpp"

namespace rfl {
namespace internal {

/// Creates a struct of type T from a tuple by copying the underlying
/// fields.
template <class T, class TupleType>
T copy_from_tuple(const TupleType& _t) {
  auto t = _t;
  return move_from_tuple<T, TupleType>(std::move(t));
}

}  // namespace internal
}  // namespace rfl

#endif
