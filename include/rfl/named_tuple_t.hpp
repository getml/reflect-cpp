#ifndef RFL_NAMED_TUPLE_T_HPP_
#define RFL_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "NamedTuple.hpp"
#include "internal/is_named_tuple.hpp"
#include "internal/ptr_named_tuple_t.hpp"
#include "internal/wrap_in_rfl_array_t.hpp"
#include "to_named_tuple.hpp"

namespace rfl {

template <class T>
struct remove_ptr;

template <internal::StringLiteral _name, class T>
struct remove_ptr<Field<_name, T>> {
  using FieldType =
      Field<_name, internal::wrap_in_rfl_array_t<
                       std::remove_cvref_t<std::remove_pointer_t<T>>>>;
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
