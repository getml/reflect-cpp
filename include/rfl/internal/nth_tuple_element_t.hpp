#ifndef RFL_INTERNAL_NTH_TUPLE_ELEMENT_T_HPP_
#define RFL_INTERNAL_NTH_TUPLE_ELEMENT_T_HPP_

#include <tuple>

#include "nth_element_t.hpp"

namespace rfl::internal {

template <int N, class T>
struct nth_tuple_element;

template <int N, class... Ts>
struct nth_tuple_element<N, std::tuple<Ts...>> {
  using Type = nth_element_t<N, Ts...>;
};

template <int N, class T>
using nth_tuple_element_t = typename nth_tuple_element<N, T>::Type;

}  // namespace rfl::internal

#endif
