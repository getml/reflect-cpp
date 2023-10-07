#ifndef RFL_NAMED_TUPLE_T_HPP_
#define RFL_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "rfl/NamedTuple.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/ptr_named_tuple_t.hpp"
#include "rfl/to_named_tuple.hpp"

namespace rfl {

template <class T>
struct remove_ptr;

template <internal::StringLiteral _name, class _Type>
struct remove_ptr<Field<_name, _Type>> {
    using FieldType = Field<_name, std::decay_t<std::remove_pointer_t<_Type>>>;
};

template <class T>
struct remove_ptrs_nt;

template <class... FieldTypes>
struct remove_ptrs_nt<NamedTuple<FieldTypes...>> {
    using NamedTupleType =
        NamedTuple<typename remove_ptr<FieldTypes>::FieldType...>;
};

/// Generates the named tuple that is equivalent to the struct T.
/// This is the result you would expect from calling to_named_tuple(my_struct).
/// All fields of the struct must be an rfl::Field.
template <class T>
using named_tuple_t =
    typename remove_ptrs_nt<internal::ptr_named_tuple_t<T>>::NamedTupleType;

}  // namespace rfl

#endif
