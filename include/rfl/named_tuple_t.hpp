#ifndef RFL_NAMED_TUPLE_T_HPP_
#define RFL_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/to_named_tuple.hpp"

namespace rfl {

/// Generates the named tuple that is equivalent to the struct T.
/// This is the result you would expect from calling to_named_tuple(my_struct).
/// All fields of the struct must be an rfl::Field.
template <class T>
using named_tuple_t =
    typename std::invoke_result<decltype(to_named_tuple<T>), T>::type;

}  // namespace rfl

#endif
