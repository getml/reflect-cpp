#ifndef RFL_APPLY_HPP_
#define RFL_APPLY_HPP_

#include <utility>

#include "Tuple.hpp"
#include "internal/tuple/apply.hpp"

namespace rfl {

/// Applies a function to a tuple (const reference version).
/// @param _f The function to apply
/// @param _tup The tuple to apply the function to
/// @return The result of applying the function to the tuple elements
template <class F, class... Types>
auto apply(F&& _f, const rfl::Tuple<Types...>& _tup) {
  return internal::tuple::apply(
      _f, _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

/// Applies a function to a tuple (mutable reference version).
/// @param _f The function to apply
/// @param _tup The tuple to apply the function to
/// @return The result of applying the function to the tuple elements
template <class F, class... Types>
auto apply(F&& _f, rfl::Tuple<Types...>& _tup) {
  return internal::tuple::apply(
      _f, _tup, std::make_integer_sequence<int, sizeof...(Types)>());
}

/// Applies a function to a tuple (rvalue reference version).
/// @param _f The function to apply
/// @param _tup The tuple to apply the function to
/// @return The result of applying the function to the tuple elements
template <class F, class... Types>
auto apply(F&& _f, rfl::Tuple<Types...>&& _tup) {
  return internal::tuple::apply(
      _f, std::move(_tup), std::make_integer_sequence<int, sizeof...(Types)>());
}

}  // namespace rfl

#endif
