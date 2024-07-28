#ifndef RFL_MAKE_FROM_TUPLE_HPP_
#define RFL_MAKE_FROM_TUPLE_HPP_

#include <utility>

#include "Tuple.hpp"
#include "internal/tuple/make_from_tuple.hpp"

namespace rfl {

template <class T, class... Types>
auto make_from_tuple(const rfl::Tuple<Types...>& _tup) {
  return internal::tuple::make_from_tuple<T>(
      _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

template <class T, class... Types>
auto make_from_tuple(rfl::Tuple<Types...>&& _tup) {
  return internal::tuple::make_from_tuple<T>(
      std::move(_tup), std::make_integer_sequence<int, sizeof...(Types)>());
}

}  // namespace rfl

#endif
