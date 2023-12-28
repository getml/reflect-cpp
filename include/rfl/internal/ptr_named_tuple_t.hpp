#ifndef RFL_INTERNAL_PTR_FIELD_TUPLE_T_HPP_
#define RFL_INTERNAL_PTR_FIELD_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "is_named_tuple.hpp"
#include "to_ptr_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using ptr_named_tuple_t =
    typename std::invoke_result<decltype(to_ptr_named_tuple<T>), T>::type;

}  // namespace internal
}  // namespace rfl

#endif
