#ifndef RFL_REMOVEFIELDS_HPP_
#define RFL_REMOVEFIELDS_HPP_

#include <algorithm>
#include <tuple>
#include <type_traits>

#include "internal/StringLiteral.hpp"
#include "internal/remove_fields.hpp"

namespace rfl {

/// Recursively removes all of the fields signified by _names from the NamedTupleType.
/// This type alias creates a new NamedTuple type with specified fields removed.
/// The removal is recursive - if the NamedTuple contains nested NamedTuples,
/// the removal will be applied to those as well.
/// @tparam NamedTupleType The NamedTuple type to remove fields from
/// @tparam _names The compile-time string literal names of fields to remove
template <class NamedTupleType, internal::StringLiteral... _names>
using remove_fields_t =
    typename internal::remove_fields<std::remove_cvref_t<NamedTupleType>,
                                     _names...>::type;

}  // namespace rfl

#endif  // RFL_REMOVEFIELDS_HPP_
