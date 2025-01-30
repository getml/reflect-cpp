#ifndef RFL_NAMED_TUPLE_T_HPP_
#define RFL_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "NamedTuple.hpp"
#include "internal/ptr_named_tuple_t.hpp"
#include "internal/remove_ptrs_nt.hpp"
#include "to_named_tuple.hpp"

namespace rfl {

/// Generates the named tuple that is equivalent to the struct T.
/// This is the result you would expect from calling to_named_tuple(my_struct).
/// All fields of the struct must be an rfl::Field.
template <class T>
using named_tuple_t = typename internal::remove_ptrs_nt<
    internal::ptr_named_tuple_t<T>>::NamedTupleType;

}  // namespace rfl

#endif
