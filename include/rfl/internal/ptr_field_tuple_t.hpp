#ifndef RFL_INTERNAL_PTR_NAMED_TUPLE_T_HPP_
#define RFL_INTERNAL_PTR_NAMED_TUPLE_T_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "to_ptr_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using ptr_field_tuple_t = decltype(to_ptr_field_tuple(std::declval<T&>()));

}  // namespace internal
}  // namespace rfl

#endif
