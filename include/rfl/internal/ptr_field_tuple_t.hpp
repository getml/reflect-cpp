#ifndef RFL_INTERNAL_PTR_NAMED_TUPLE_T_HPP_
#define RFL_INTERNAL_PTR_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/to_ptr_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using ptr_field_tuple_t =
    typename std::invoke_result<decltype(to_ptr_field_tuple<T>), T>::type;

}  // namespace internal
}  // namespace rfl

#endif
