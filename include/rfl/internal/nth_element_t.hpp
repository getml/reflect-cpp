#ifndef RFL_INTERNAL_NTH_ELEMENT_T_HPP_
#define RFL_INTERNAL_NTH_ELEMENT_T_HPP_

#include <tuple>

namespace rfl::internal {

#if defined(__clang__)

template <int N, class... Ts>
using nth_element_t = __type_pack_element<N, Ts...>;

#else

template <int N, class... Ts>
struct NthElement;

template <int N, class Head, class... Tail>
struct NthElement<N, Head, Tail...> {
  using Type = typename NthElement<N - 1, Tail...>::Type;
};

template <class T0, class... Ts>
struct NthElement<0, T0, Ts...> {
  using Type = T0;
};

template <class T0, class T1, class... Ts>
struct NthElement<1, T0, T1, Ts...> {
  using Type = T1;
};

template <class T0, class T1, class T2, class... Ts>
struct NthElement<2, T0, T1, T2, Ts...> {
  using Type = T2;
};

template <class T0, class T1, class T2, class T3, class... Ts>
struct NthElement<3, T0, T1, T2, T3, Ts...> {
  using Type = T3;
};

template <class T0, class T1, class T2, class T3, class T4, class... Ts>
struct NthElement<4, T0, T1, T2, T3, T4, Ts...> {
  using Type = T4;
};

template <class T0, class T1, class T2, class T3, class T4, class T5,
          class... Ts>
struct NthElement<5, T0, T1, T2, T3, T4, T5, Ts...> {
  using Type = T5;
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class... Ts>
struct NthElement<6, T0, T1, T2, T3, T4, T5, T6, Ts...> {
  using Type = T6;
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class... Ts>
struct NthElement<7, T0, T1, T2, T3, T4, T5, T6, T7, Ts...> {
  using Type = T7;
};

template <int N, class... Ts>
using nth_element_t = typename NthElement<N, Ts...>::Type;

#endif

}  // namespace rfl::internal

#endif
