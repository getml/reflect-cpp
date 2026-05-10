#ifndef RFL_TUPLE_CAT_HPP_
#define RFL_TUPLE_CAT_HPP_

#include "Tuple.hpp"
#include "internal/tuple/concat.hpp"

namespace rfl {

/// Concatenates multiple tuples (const reference version).
/// @param _head The first tuple
/// @param _tail The remaining tuples to concatenate
/// @return A new tuple containing all elements from all input tuples
template <class Head, class... Tail>
auto tuple_cat(const Head& _head, const Tail&... _tail) {
  return internal::tuple::concat(_head, _tail...);
}

/// Concatenates multiple tuples (rvalue reference version).
/// @param _head The first tuple
/// @param _tail The remaining tuples to concatenate
/// @return A new tuple containing all elements from all input tuples
template <class Head, class... Tail>
auto tuple_cat(Head&& _head, Tail&&... _tail) {
  return internal::tuple::concat(std::forward<Head>(_head),
                                 std::forward<Tail>(_tail)...);
}

/// Concatenates zero tuples, returning an empty tuple.
/// @return An empty tuple
inline auto tuple_cat() { return rfl::Tuple(); }

}  // namespace rfl

#endif
