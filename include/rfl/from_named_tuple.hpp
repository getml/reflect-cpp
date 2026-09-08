#ifndef RFL_FROM_NAMED_TUPLE_HPP_
#define RFL_FROM_NAMED_TUPLE_HPP_

#include <type_traits>

#include "internal/copy_from_named_tuple.hpp"
#include "internal/copy_from_tuple.hpp"
#include "internal/has_fields.hpp"
#include "internal/move_from_named_tuple.hpp"
#include "internal/move_from_tuple.hpp"
#include "named_tuple_t.hpp"

namespace rfl {

/// Generates the struct T from a named tuple (rvalue reference version).
/// This converts a named tuple (tuple-like structure with compile-time string names)
/// back into the original struct type. If the named tuple type doesn't match exactly,
/// it will be converted first. For structs with rfl::Field members, values are moved/copied
/// to the fields; for plain structs, values are used for aggregate initialization.
/// @tparam T The struct type to construct
/// @tparam NamedTupleType The type of the named tuple
/// @param _n The named tuple to convert from (will be moved if rvalue)
/// @return An instance of struct T
template <class T, class NamedTupleType>
auto from_named_tuple(NamedTupleType&& _n) {
  using RequiredType = std::remove_cvref_t<rfl::named_tuple_t<T>>;
  if constexpr (!std::is_same<std::remove_cvref_t<NamedTupleType>,
                              RequiredType>()) {
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

/// Generates the struct T from a named tuple (const reference version).
/// This converts a named tuple (tuple-like structure with compile-time string names)
/// back into the original struct type. For structs with rfl::Field members, values are copied
/// to the fields; for plain structs, values are used for aggregate initialization.
/// @tparam T The struct type to construct
/// @tparam NamedTupleType The type of the named tuple
/// @param _n The named tuple to convert from (const reference)
/// @return An instance of struct T
template <class T, class NamedTupleType>
auto from_named_tuple(const NamedTupleType& _n) {
  using RequiredType = std::remove_cvref_t<rfl::named_tuple_t<T>>;
  if constexpr (!std::is_same<std::remove_cvref_t<NamedTupleType>,
                              RequiredType>()) {
    return from_named_tuple<T>(RequiredType(_n));
  } else if constexpr (internal::has_fields<T>()) {
    return internal::copy_from_named_tuple<T>(_n);
  } else {
    return internal::copy_from_tuple<T>(_n.values());
  }
}

}  // namespace rfl

#endif
