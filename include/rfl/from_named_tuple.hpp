#ifndef RFL_FROM_NAMED_TUPLE_HPP_
#define RFL_FROM_NAMED_TUPLE_HPP_

#include <type_traits>

#include "rfl/internal/copy_from_named_tuple.hpp"
#include "rfl/internal/copy_from_tuple.hpp"
#include "rfl/internal/has_fields.hpp"
#include "rfl/internal/move_from_named_tuple.hpp"
#include "rfl/internal/move_from_tuple.hpp"
#include "rfl/named_tuple_t.hpp"

namespace rfl {

/// Generates the struct T from a named tuple.
template <class T, class NamedTupleType>
T from_named_tuple(NamedTupleType&& _n) {
  using RequiredType = std::decay_t<rfl::named_tuple_t<T>>;
  if constexpr (!std::is_same<std::decay_t<NamedTupleType>, RequiredType>()) {
    return from_named_tuple<T>(RequiredType(std::forward<NamedTupleType>(_n)));
  } else if constexpr (internal::has_fields<T>()) {
    if constexpr (std::is_lvalue_reference<NamedTupleType>{}) {
      return internal::copy_from_named_tuple<T>(_n);
    } else {
      return internal::move_from_named_tuple<T>(_n);
    }
  } else {
    if constexpr (std::is_lvalue_reference<NamedTupleType>{}) {
      return internal::copy_from_tuple<T>(_n.values());
    } else {
      return internal::move_from_tuple<T>(std::move(_n.values()));
    }
  }
}

/// Generates the struct T from a named tuple.
template <class T, class NamedTupleType>
T from_named_tuple(const NamedTupleType& _n) {
  using RequiredType = std::decay_t<rfl::named_tuple_t<T>>;
  if constexpr (!std::is_same<std::decay_t<NamedTupleType>, RequiredType>()) {
    return from_named_tuple<T>(RequiredType(_n));
  } else if constexpr (internal::has_fields<T>()) {
    return internal::copy_from_named_tuple<T>(_n);
  } else {
    return internal::copy_from_tuple<T>(_n.values());
  }
}

}  // namespace rfl

#endif
