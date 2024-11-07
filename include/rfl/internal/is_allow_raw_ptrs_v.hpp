#ifndef RFL_INTERNAL_ISALLOWRAWPTRS_HPP_
#define RFL_INTERNAL_ISALLOWRAWPTRS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../AllowRawPtrs.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_allow_raw_ptrs;

template <class T>
class is_allow_raw_ptrs : public std::false_type {};

template <>
class is_allow_raw_ptrs<AllowRawPtrs> : public std::true_type {};

template <class T>
constexpr bool is_allow_raw_ptrs_v =
    is_allow_raw_ptrs<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
