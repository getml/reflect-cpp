#ifndef RFL_INTERNAL_TUPLE_MAKE_FROM_TUPLE_HPP_
#define RFL_INTERNAL_TUPLE_MAKE_FROM_TUPLE_HPP_

#include <utility>

#include "../../Tuple.hpp"

namespace rfl::internal::tuple {

template <class T, class... Types, int... _is>
T make_from_tuple(const rfl::Tuple<Types...>& _t1,
                  std::integer_sequence<int, _is...>) {
  return T{rfl::get<_is>(_t1)...};
}

template <class T, class... Types, int... _is>
T make_from_tuple(rfl::Tuple<Types...>&& _t1,
                  std::integer_sequence<int, _is...>) {
  return T{std::move(rfl::get<_is>(_t1))...};
}

}  // namespace rfl::internal::tuple

#endif
