#ifndef RFL_INTERNAL_TUPLE_APPLY_HPP_
#define RFL_INTERNAL_TUPLE_APPLY_HPP_

#include <utility>

#include "../../Tuple.hpp"

namespace rfl::internal::tuple {

template <class F, class... Types, int... _is>
auto apply(F&& _f, const rfl::Tuple<Types...>& _tup,
           std::integer_sequence<int, _is...>) {
  return _f(rfl::get<_is>(_tup)...);
}

template <class F, class... Types, int... _is>
auto apply(F&& _f, rfl::Tuple<Types...>& _tup,
           std::integer_sequence<int, _is...>) {
  return _f(rfl::get<_is>(_tup)...);
}

template <class F, class... Types, int... _is>
auto apply(F&& _f, rfl::Tuple<Types...>&& _tup,
           std::integer_sequence<int, _is...>) {
  return _f(std::move(rfl::get<_is>(_tup))...);
}

}  // namespace rfl::internal::tuple

#endif
