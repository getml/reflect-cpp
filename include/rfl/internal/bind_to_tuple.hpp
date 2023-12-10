#ifndef RFL_INTERNAL_BIND_TO_TUPLE_HELPER_HPP_
#define RFL_INTERNAL_BIND_TO_TUPLE_HELPER_HPP_

#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "rfl/always_false.hpp"
#include "rfl/internal/has_n_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
constexpr auto tuple_view(T&);

template <class T, typename F>
constexpr auto bind_to_tuple(T& _t, F&& f) {
  auto view = tuple_view(_t);
  return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    return std::make_tuple(std::forward<F>(f)(std::get<Is>(view))...);
  }(std::make_index_sequence<std::tuple_size_v<decltype(view)>>());
}

/*The following boilerplate code was generated using a Python script:
def make_field_template(num: int) -> str:
    fields = ", ".join([f"f{i+1}" for i in range(num)])
    ref_fields = ", ".join([f"std::ref(f{i+1})" for i in range(num)])
    return (
        """
  } else if constexpr (has_n_fields<T, """
        + str(num)
        + """>) {"""
        + ("auto& [" + fields + "] = _t;" if num > 0 else "")
        + """
  return std::make_tuple("""
        + ref_fields
        + ");"
    )

code = "".join((make_field_template(i) for i in range(101)))

with open("generated_code4.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)
*/

template <class T>
constexpr auto tuple_view(T& _t) {
  if constexpr (has_n_fields<T, 0>) {
    return std::make_tuple();
  } else if constexpr (has_n_fields<T, 1>) {
    auto& [f1] = _t;
    return std::make_tuple(std::ref(f1));
  } else if constexpr (has_n_fields<T, 2>) {
    auto& [f1, f2] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2));
  } else if constexpr (has_n_fields<T, 3>) {
    auto& [f1, f2, f3] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3));
  } else if constexpr (has_n_fields<T, 4>) {
    auto& [f1, f2, f3, f4] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4));
  } else if constexpr (has_n_fields<T, 5>) {
    auto& [f1, f2, f3, f4, f5] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5));
  } else if constexpr (has_n_fields<T, 6>) {
    auto& [f1, f2, f3, f4, f5, f6] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6));
  } else if constexpr (has_n_fields<T, 7>) {
    auto& [f1, f2, f3, f4, f5, f6, f7] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7));
  } else if constexpr (has_n_fields<T, 8>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7), std::ref(f8));
  } else if constexpr (has_n_fields<T, 9>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7), std::ref(f8), std::ref(f9));
  } else if constexpr (has_n_fields<T, 10>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10));
  } else if constexpr (has_n_fields<T, 11>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7), std::ref(f8), std::ref(f9),
                           std::ref(f10), std::ref(f11));
  } else if constexpr (has_n_fields<T, 12>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12] = _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7), std::ref(f8), std::ref(f9),
                           std::ref(f10), std::ref(f11), std::ref(f12));
  } else if constexpr (has_n_fields<T, 13>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13));
  } else if constexpr (has_n_fields<T, 14>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14));
  } else if constexpr (has_n_fields<T, 15>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15] =
        _t;
    return std::make_tuple(std::ref(f1), std::ref(f2), std::ref(f3),
                           std::ref(f4), std::ref(f5), std::ref(f6),
                           std::ref(f7), std::ref(f8), std::ref(f9),
                           std::ref(f10), std::ref(f11), std::ref(f12),
                           std::ref(f13), std::ref(f14), std::ref(f15));
  } else if constexpr (has_n_fields<T, 16>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16));
  } else if constexpr (has_n_fields<T, 17>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17));
  } else if constexpr (has_n_fields<T, 18>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18));
  } else if constexpr (has_n_fields<T, 19>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19));
  } else if constexpr (has_n_fields<T, 20>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20));
  } else if constexpr (has_n_fields<T, 21>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21));
  } else if constexpr (has_n_fields<T, 22>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22));
  } else if constexpr (has_n_fields<T, 23>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23));
  } else if constexpr (has_n_fields<T, 24>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24));
  } else if constexpr (has_n_fields<T, 25>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25));
  } else if constexpr (has_n_fields<T, 26>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26));
  } else if constexpr (has_n_fields<T, 27>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27));
  } else if constexpr (has_n_fields<T, 28>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28));
  } else if constexpr (has_n_fields<T, 29>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
           f29] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29));
  } else if constexpr (has_n_fields<T, 30>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30));
  } else if constexpr (has_n_fields<T, 31>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31));
  } else if constexpr (has_n_fields<T, 32>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32));
  } else if constexpr (has_n_fields<T, 33>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33));
  } else if constexpr (has_n_fields<T, 34>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34));
  } else if constexpr (has_n_fields<T, 35>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35));
  } else if constexpr (has_n_fields<T, 36>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36));
  } else if constexpr (has_n_fields<T, 37>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37));
  } else if constexpr (has_n_fields<T, 38>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38));
  } else if constexpr (has_n_fields<T, 39>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39));
  } else if constexpr (has_n_fields<T, 40>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40));
  } else if constexpr (has_n_fields<T, 41>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41));
  } else if constexpr (has_n_fields<T, 42>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42));
  } else if constexpr (has_n_fields<T, 43>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42,
           f43] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43));
  } else if constexpr (has_n_fields<T, 44>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44));
  } else if constexpr (has_n_fields<T, 45>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45));
  } else if constexpr (has_n_fields<T, 46>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46));
  } else if constexpr (has_n_fields<T, 47>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47));
  } else if constexpr (has_n_fields<T, 48>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48));
  } else if constexpr (has_n_fields<T, 49>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49));
  } else if constexpr (has_n_fields<T, 50>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50));
  } else if constexpr (has_n_fields<T, 51>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51));
  } else if constexpr (has_n_fields<T, 52>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52));
  } else if constexpr (has_n_fields<T, 53>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53));
  } else if constexpr (has_n_fields<T, 54>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54));
  } else if constexpr (has_n_fields<T, 55>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55));
  } else if constexpr (has_n_fields<T, 56>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56));
  } else if constexpr (has_n_fields<T, 57>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56,
           f57] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57));
  } else if constexpr (has_n_fields<T, 58>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58));
  } else if constexpr (has_n_fields<T, 59>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59));
  } else if constexpr (has_n_fields<T, 60>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60));
  } else if constexpr (has_n_fields<T, 61>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61));
  } else if constexpr (has_n_fields<T, 62>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62));
  } else if constexpr (has_n_fields<T, 63>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63));
  } else if constexpr (has_n_fields<T, 64>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64));
  } else if constexpr (has_n_fields<T, 65>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65));
  } else if constexpr (has_n_fields<T, 66>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66));
  } else if constexpr (has_n_fields<T, 67>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67));
  } else if constexpr (has_n_fields<T, 68>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68));
  } else if constexpr (has_n_fields<T, 69>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69));
  } else if constexpr (has_n_fields<T, 70>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70));
  } else if constexpr (has_n_fields<T, 71>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70,
           f71] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71));
  } else if constexpr (has_n_fields<T, 72>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72));
  } else if constexpr (has_n_fields<T, 73>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73));
  } else if constexpr (has_n_fields<T, 74>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74));
  } else if constexpr (has_n_fields<T, 75>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75));
  } else if constexpr (has_n_fields<T, 76>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76));
  } else if constexpr (has_n_fields<T, 77>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77));
  } else if constexpr (has_n_fields<T, 78>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78));
  } else if constexpr (has_n_fields<T, 79>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79));
  } else if constexpr (has_n_fields<T, 80>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80));
  } else if constexpr (has_n_fields<T, 81>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81));
  } else if constexpr (has_n_fields<T, 82>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82));
  } else if constexpr (has_n_fields<T, 83>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83));
  } else if constexpr (has_n_fields<T, 84>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84));
  } else if constexpr (has_n_fields<T, 85>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84,
           f85] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85));
  } else if constexpr (has_n_fields<T, 86>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86));
  } else if constexpr (has_n_fields<T, 87>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87));
  } else if constexpr (has_n_fields<T, 88>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88));
  } else if constexpr (has_n_fields<T, 89>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89));
  } else if constexpr (has_n_fields<T, 90>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90));
  } else if constexpr (has_n_fields<T, 91>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91));
  } else if constexpr (has_n_fields<T, 92>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92));
  } else if constexpr (has_n_fields<T, 93>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93));
  } else if constexpr (has_n_fields<T, 94>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94));
  } else if constexpr (has_n_fields<T, 95>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95));
  } else if constexpr (has_n_fields<T, 96>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95), std::ref(f96));
  } else if constexpr (has_n_fields<T, 97>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95), std::ref(f96), std::ref(f97));
  } else if constexpr (has_n_fields<T, 98>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98] =
        _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95), std::ref(f96), std::ref(f97), std::ref(f98));
  } else if constexpr (has_n_fields<T, 99>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98,
           f99] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95), std::ref(f96), std::ref(f97), std::ref(f98),
        std::ref(f99));
  } else if constexpr (has_n_fields<T, 100>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98, f99,
           f100] = _t;
    return std::make_tuple(
        std::ref(f1), std::ref(f2), std::ref(f3), std::ref(f4), std::ref(f5),
        std::ref(f6), std::ref(f7), std::ref(f8), std::ref(f9), std::ref(f10),
        std::ref(f11), std::ref(f12), std::ref(f13), std::ref(f14),
        std::ref(f15), std::ref(f16), std::ref(f17), std::ref(f18),
        std::ref(f19), std::ref(f20), std::ref(f21), std::ref(f22),
        std::ref(f23), std::ref(f24), std::ref(f25), std::ref(f26),
        std::ref(f27), std::ref(f28), std::ref(f29), std::ref(f30),
        std::ref(f31), std::ref(f32), std::ref(f33), std::ref(f34),
        std::ref(f35), std::ref(f36), std::ref(f37), std::ref(f38),
        std::ref(f39), std::ref(f40), std::ref(f41), std::ref(f42),
        std::ref(f43), std::ref(f44), std::ref(f45), std::ref(f46),
        std::ref(f47), std::ref(f48), std::ref(f49), std::ref(f50),
        std::ref(f51), std::ref(f52), std::ref(f53), std::ref(f54),
        std::ref(f55), std::ref(f56), std::ref(f57), std::ref(f58),
        std::ref(f59), std::ref(f60), std::ref(f61), std::ref(f62),
        std::ref(f63), std::ref(f64), std::ref(f65), std::ref(f66),
        std::ref(f67), std::ref(f68), std::ref(f69), std::ref(f70),
        std::ref(f71), std::ref(f72), std::ref(f73), std::ref(f74),
        std::ref(f75), std::ref(f76), std::ref(f77), std::ref(f78),
        std::ref(f79), std::ref(f80), std::ref(f81), std::ref(f82),
        std::ref(f83), std::ref(f84), std::ref(f85), std::ref(f86),
        std::ref(f87), std::ref(f88), std::ref(f89), std::ref(f90),
        std::ref(f91), std::ref(f92), std::ref(f93), std::ref(f94),
        std::ref(f95), std::ref(f96), std::ref(f97), std::ref(f98),
        std::ref(f99), std::ref(f100));

    // ---------------------------------
    // End of generated boilerplate code
    // ---------------------------------
  } else {
    static_assert(
        rfl::always_false_v<T>,
        "\n\nThis error occurs for one of two reasons:\n\n"
        "1) You have created a struct with more than 100 fields, which is "
        "unsupported. Please split up your struct into several "
        "smaller structs and then use rfl::Flatten<...> to combine them. "
        "Refer "
        "to the documentation on rfl::Flatten<...> for details.\n\n"
        "2) You have added a custom constructor to your struct, which you "
        "shouldn't do either. Please refer to the sections on custom "
        "classes or custom parsers in the documentation "
        "for solutions to this problem.\n\n");
  }
}

}  // namespace internal
}  // namespace rfl

#endif
