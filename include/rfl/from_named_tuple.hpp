#ifndef RFL_FROM_NAMED_TUPLE_HPP_
#define RFL_FROM_NAMED_TUPLE_HPP_

#include <type_traits>

#include "internal/copy_from_named_tuple.hpp"
#include "internal/move_from_named_tuple.hpp"

namespace rfl {

/// Generates the struct T from a named tuple.
template <class T, class NamedTupleType>
T from_named_tuple(NamedTupleType&& _n) {
    if constexpr (std::is_lvalue_reference<NamedTupleType>{}) {
        return internal::copy_from_named_tuple<T>(_n);
    } else {
        return internal::move_from_named_tuple<T>(_n);
    }
}

/// Generates the struct T from a named tuple.
template <class T, class NamedTupleType>
T from_named_tuple(const NamedTupleType& _n) {
    return internal::copy_from_named_tuple<T>(_n);
}

}  // namespace rfl

#endif
