#ifndef RFL_INTERNAL_ALLOWRAWPTRS_HPP_
#define RFL_INTERNAL_ALLOWRAWPTRS_HPP_

#include <type_traits>

#include "../AllowRawPtrs.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class allow_raw_ptrs;

template <class T>
class allow_raw_ptrs : public std::false_type {};

template <>
class allow_raw_ptrs<AllowRawPtrs> : public std::true_type {};

template <class Head, class... Tail>
struct allow_raw_ptrs<Processors<Head, Tail...>> {
  static constexpr bool value =
      (allow_raw_ptrs<Head>::value || ... || allow_raw_ptrs<Tail>::value);
};

template <class T>
constexpr bool allow_raw_ptrs_v =
    allow_raw_ptrs<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
