#ifndef RFL_REPLACE_HPP_
#define RFL_REPLACE_HPP_

#include <type_traits>

#include "from_named_tuple.hpp"

namespace rfl {

/// Replaces one or several fields, returning a new version
/// with the non-replaced fields left unchanged.
template <class T, class RField, class... OtherRFields>
auto replace(T&& _t, RField&& _field, OtherRFields&&... _other_fields) {
    if constexpr (internal::is_named_tuple_v<T>) {
        return std::forward<T>(_t).replace(
            to_named_tuple(std::forward<RField>(_field)),
            to_named_tuple(std::forward<OtherRFields>(_other_fields))...);
    } else {
        return from_named_tuple<T>(
            to_named_tuple(std::forward<T>(_t))
                .replace(to_named_tuple(std::forward<RField>(_field)),
                         to_named_tuple(
                             std::forward<OtherRFields>(_other_fields))...));
    }
}

/// Replaces one or several fields, returning a new version
/// with the non-replaced fields left unchanged.
template <class T, class RField, class... OtherRFields>
auto replace(const T& _t, RField&& _field, OtherRFields&&... _other_fields) {
    if constexpr (internal::is_named_tuple_v<T>) {
        return _t.replace(
            to_named_tuple(std::forward<RField>(_field)),
            to_named_tuple(std::forward<OtherRFields>(_other_fields))...);
    } else {
        return from_named_tuple<T>(to_named_tuple(_t).replace(
            to_named_tuple(std::forward<RField>(_field)),
            to_named_tuple(std::forward<OtherRFields>(_other_fields))...));
    }
}

}  // namespace rfl

#endif
