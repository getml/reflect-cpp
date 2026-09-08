#ifndef RFL_MAKE_FROM_TUPLE_HPP_
#define RFL_MAKE_FROM_TUPLE_HPP_

#include <utility>

#include "Tuple.hpp"
#include "internal/tuple/make_from_tuple.hpp"

namespace rfl {

/// Constructs an object of type T from a tuple (const reference version).
/// @param _tup The tuple containing the values to construct T with
/// @return A new object of type T constructed from the tuple elements
template <class T, class... Types>
auto make_from_tuple(const rfl::Tuple<Types...>& _tup) {
  return internal::tuple::make_from_tuple<T>(
      _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

/// Constructs an object of type T from a tuple (rvalue reference version).
/// @param _tup The tuple containing the values to construct T with
/// @return A new object of type T constructed from the tuple elements
template <class T, class... Types>
auto make_from_tuple(rfl::Tuple<Types...>&& _tup) {
  return internal::tuple::make_from_tuple<T>(
      std::move(_tup), std::make_integer_sequence<int, sizeof...(Types)>());
}

}  // namespace rfl

#endif
