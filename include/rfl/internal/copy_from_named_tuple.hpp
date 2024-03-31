#ifndef RFL_INTERNAL_COPY_FROM_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_COPY_FROM_NAMED_TUPLE_HPP_

#include "move_from_named_tuple.hpp"

namespace rfl {
namespace internal {

/// Creates a struct of type T from a named tuple.
/// All fields of the struct must be an rfl::Field.
template <class T, class NamedTupleType>
T copy_from_named_tuple(const NamedTupleType& _n) {
  auto n = _n;
  return move_from_named_tuple<T>(std::move(n));
}

}  // namespace internal
}  // namespace rfl

#endif
