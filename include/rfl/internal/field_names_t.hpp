#ifndef RFL_INTERNAL_FIELD_NAMES_T_HPP_
#define RFL_INTERNAL_FIELD_NAMES_T_HPP_

#include <functional>
#include <type_traits>

#include "rfl/internal/to_ptr_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using field_names_t =
    typename std::invoke_result<decltype(to_ptr_tuple<T>), T>::type;

}  // namespace internal
}  // namespace rfl

#endif
