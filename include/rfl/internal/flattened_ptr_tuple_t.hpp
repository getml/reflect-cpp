#ifndef RFL_INTERNAL_FLATTENED_PTR_TUPLE_T_HPP_
#define RFL_INTERNAL_FLATTENED_PTR_TUPLE_T_HPP_

#include <functional>
#include <type_traits>

#include "to_flattened_ptr_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using flattened_ptr_tuple_t =
    typename std::invoke_result<decltype(to_flattened_ptr_tuple<T>), T>::type;

}  // namespace internal
}  // namespace rfl

#endif
