#ifndef RFL_INTERNAL_ELEMENT_INDEX_HPP_
#define RFL_INTERNAL_ELEMENT_INDEX_HPP_

#include <type_traits>

namespace rfl::internal {

template <int _i, class T>
consteval int find_element_index() {
  return -1;
}

template <int _i, class T, class Head, class... Tail>
consteval int find_element_index() {
  if constexpr (std::is_same_v<T, Head>) {
    return _i;
  } else {
    return find_element_index<_i + 1, T, Tail...>();
  }
}

template <class T, class... AlternativeTypes>
consteval int element_index() {
  return find_element_index<0, T, AlternativeTypes...>();
}

}  // namespace rfl::internal

#endif
