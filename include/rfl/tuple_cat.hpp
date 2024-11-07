#ifndef RFL_TUPLE_CAT_HPP_
#define RFL_TUPLE_CAT_HPP_

#include "Tuple.hpp"
#include "internal/tuple/concat.hpp"

namespace rfl {

template <class Head, class... Tail>
auto tuple_cat(const Head& _head, const Tail&... _tail) {
  return internal::tuple::concat(_head, _tail...);
}

template <class Head, class... Tail>
auto tuple_cat(Head&& _head, Tail&&... _tail) {
  return internal::tuple::concat(std::forward<Head>(_head),
                                 std::forward<Tail>(_tail)...);
}

inline auto tuple_cat() { return rfl::Tuple(); }

}  // namespace rfl

#endif
