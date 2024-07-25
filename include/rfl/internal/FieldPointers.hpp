#ifndef RFL_INTERNAL_FIELDPOINTERS_HPP_
#define RFL_INTERNAL_FIELDPOINTERS_HPP_

#include <array>

#include "nth_element_t.hpp"

namespace rfl {
namespace internal {

template <class PtrType, class... Types>
struct FieldPointers {
  std::array<PtrType*, sizeof...(Types)> ptrs_;

  template <int _i>
  constexpr auto get() {
    return (nth_element_t<_i, Types...>*)(std::get<_i>(ptrs_));
  }
};

}  // namespace internal
}  // namespace rfl

#endif
