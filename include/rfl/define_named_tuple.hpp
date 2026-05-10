#ifndef RFL_DEFINENAMEDTUPLE_HPP_
#define RFL_DEFINENAMEDTUPLE_HPP_

#include "NamedTuple.hpp"
#include "internal/define_named_tuple.hpp"

namespace rfl {

/// Combines several Field types into a NamedTuple type.
/// This is a type alias for creating NamedTuple types from a list of Field types.
/// A NamedTuple is like a struct but with compile-time field name reflection and runtime access.
/// Example: define_named_tuple_t<Field<"x", int>, Field<"y", double>> creates a NamedTuple with fields x and y.
/// @tparam FieldTypes The Field types that will make up the NamedTuple
template <class... FieldTypes>
using define_named_tuple_t =
    typename internal::define_named_tuple<FieldTypes...>::type;

}  // namespace rfl

#endif  // RFL_DEFINENAMEDTUPLE_HPP_
