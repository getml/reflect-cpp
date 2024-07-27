#ifndef RFL_APPLY_HPP_
#define RFL_APPLY_HPP_

#include <utility>

#include "Tuple.hpp"
#include "internal/tuple/apply.hpp"

namespace rfl {

template <class F, class... Types>
auto apply(F&& _f, const rfl::Tuple<Types...>& _tup) {
  return internal::tuple::apply(
      _f, _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

template <class F, class... Types>
auto apply(F&& _f, rfl::Tuple<Types...>& _tup) {
  return internal::tuple::apply(
      _f, _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

template <class F, class... Types>
auto apply(F&& _f, rfl::Tuple<Types...>&& _tup) {
  return internal::tuple::apply(
      _f, std::move(_tup), std::make_integer_sequence<int, sizeof...(Types)>());
}

}  // namespace rfl

#endif
