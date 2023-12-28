#ifndef RFL_INTERNAL_TUPLE_T_HPP_
#define RFL_INTERNAL_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "ptr_tuple_t.hpp"
#include "remove_ptrs_tup.hpp"
#include "../to_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using tuple_t = typename remove_ptrs_tup<ptr_tuple_t<T>>::TupleType;

}  // namespace internal
}  // namespace rfl

#endif
