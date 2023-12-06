#ifndef RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_IMPL_HPP_
#define RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_IMPL_HPP_

#include <tuple>
#include <type_traits>

#include "rfl/always_false.hpp"
#include "rfl/internal/has_n_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"

namespace rfl {
namespace internal {

/*The following boilerplate code was generated using a Python script:
def make_field_template(num: int) -> str:
    fields = ", ".join([f"f{i+1}" for i in range(num)])
    move_fields = ", ".join([f"std::move(f{i+1})" for i in range(num)])
    return (
        """
  } else if constexpr (internal::has_n_fields<T, """
        + str(num)
        + """>) {"""
        + ("auto& [" + fields + "] = _t;" if num > 0 else "")
        + """
  return std::make_tuple("""
        + move_fields
        + ");"
    )


beginning = """
if constexpr (internal::is_named_tuple_v<T>) {
    return _t.fields();"""

main_part = "".join((make_field_template(i) for i in range(101)))

code = beginning + main_part

with open("generated_code2.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)
*/

template <class OriginalStruct>
auto move_to_field_tuple_impl(OriginalStruct&& _t) {
  using T = std::decay_t<OriginalStruct>;
  if constexpr (internal::is_named_tuple_v<T>) {
    return _t.fields();
  } else if constexpr (internal::has_n_fields<T, 0>) {
    return std::make_tuple();
  } else if constexpr (internal::has_n_fields<T, 1>) {
    auto& [f1] = _t;
    return std::make_tuple(std::move(f1));
  } else if constexpr (internal::has_n_fields<T, 2>) {
    auto& [f1, f2] = _t;
    return std::make_tuple(std::move(f1), std::move(f2));
  } else if constexpr (internal::has_n_fields<T, 3>) {
    auto& [f1, f2, f3] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3));
  } else if constexpr (internal::has_n_fields<T, 4>) {
    auto& [f1, f2, f3, f4] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4));
  } else if constexpr (internal::has_n_fields<T, 5>) {
    auto& [f1, f2, f3, f4, f5] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5));
  } else if constexpr (internal::has_n_fields<T, 6>) {
    auto& [f1, f2, f3, f4, f5, f6] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6));
  } else if constexpr (internal::has_n_fields<T, 7>) {
    auto& [f1, f2, f3, f4, f5, f6, f7] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7));
  } else if constexpr (internal::has_n_fields<T, 8>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8));
  } else if constexpr (internal::has_n_fields<T, 9>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9));
  } else if constexpr (internal::has_n_fields<T, 10>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10));
  } else if constexpr (internal::has_n_fields<T, 11>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11));
  } else if constexpr (internal::has_n_fields<T, 12>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12));
  } else if constexpr (internal::has_n_fields<T, 13>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13));
  } else if constexpr (internal::has_n_fields<T, 14>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13), std::move(f14));
  } else if constexpr (internal::has_n_fields<T, 15>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15] =
        _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13), std::move(f14), std::move(f15));
  } else if constexpr (internal::has_n_fields<T, 16>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16));
  } else if constexpr (internal::has_n_fields<T, 17>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13), std::move(f14), std::move(f15),
                           std::move(f16), std::move(f17));
  } else if constexpr (internal::has_n_fields<T, 18>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13), std::move(f14), std::move(f15),
                           std::move(f16), std::move(f17), std::move(f18));
  } else if constexpr (internal::has_n_fields<T, 19>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19));
  } else if constexpr (internal::has_n_fields<T, 20>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20));
  } else if constexpr (internal::has_n_fields<T, 21>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21] = _t;
    return std::make_tuple(std::move(f1), std::move(f2), std::move(f3),
                           std::move(f4), std::move(f5), std::move(f6),
                           std::move(f7), std::move(f8), std::move(f9),
                           std::move(f10), std::move(f11), std::move(f12),
                           std::move(f13), std::move(f14), std::move(f15),
                           std::move(f16), std::move(f17), std::move(f18),
                           std::move(f19), std::move(f20), std::move(f21));
  } else if constexpr (internal::has_n_fields<T, 22>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22));
  } else if constexpr (internal::has_n_fields<T, 23>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23));
  } else if constexpr (internal::has_n_fields<T, 24>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24));
  } else if constexpr (internal::has_n_fields<T, 25>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25));
  } else if constexpr (internal::has_n_fields<T, 26>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26));
  } else if constexpr (internal::has_n_fields<T, 27>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27));
  } else if constexpr (internal::has_n_fields<T, 28>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28));
  } else if constexpr (internal::has_n_fields<T, 29>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
           f29] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29));
  } else if constexpr (internal::has_n_fields<T, 30>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30));
  } else if constexpr (internal::has_n_fields<T, 31>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31));
  } else if constexpr (internal::has_n_fields<T, 32>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32));
  } else if constexpr (internal::has_n_fields<T, 33>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33));
  } else if constexpr (internal::has_n_fields<T, 34>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34));
  } else if constexpr (internal::has_n_fields<T, 35>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35));
  } else if constexpr (internal::has_n_fields<T, 36>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36));
  } else if constexpr (internal::has_n_fields<T, 37>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37));
  } else if constexpr (internal::has_n_fields<T, 38>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38));
  } else if constexpr (internal::has_n_fields<T, 39>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39));
  } else if constexpr (internal::has_n_fields<T, 40>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40));
  } else if constexpr (internal::has_n_fields<T, 41>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41));
  } else if constexpr (internal::has_n_fields<T, 42>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42));
  } else if constexpr (internal::has_n_fields<T, 43>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42,
           f43] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43));
  } else if constexpr (internal::has_n_fields<T, 44>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44));
  } else if constexpr (internal::has_n_fields<T, 45>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45));
  } else if constexpr (internal::has_n_fields<T, 46>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46));
  } else if constexpr (internal::has_n_fields<T, 47>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47));
  } else if constexpr (internal::has_n_fields<T, 48>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48));
  } else if constexpr (internal::has_n_fields<T, 49>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49));
  } else if constexpr (internal::has_n_fields<T, 50>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50));
  } else if constexpr (internal::has_n_fields<T, 51>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51));
  } else if constexpr (internal::has_n_fields<T, 52>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52));
  } else if constexpr (internal::has_n_fields<T, 53>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53));
  } else if constexpr (internal::has_n_fields<T, 54>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54));
  } else if constexpr (internal::has_n_fields<T, 55>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55));
  } else if constexpr (internal::has_n_fields<T, 56>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56));
  } else if constexpr (internal::has_n_fields<T, 57>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56,
           f57] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57));
  } else if constexpr (internal::has_n_fields<T, 58>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58));
  } else if constexpr (internal::has_n_fields<T, 59>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59));
  } else if constexpr (internal::has_n_fields<T, 60>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60));
  } else if constexpr (internal::has_n_fields<T, 61>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61));
  } else if constexpr (internal::has_n_fields<T, 62>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62));
  } else if constexpr (internal::has_n_fields<T, 63>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63));
  } else if constexpr (internal::has_n_fields<T, 64>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64));
  } else if constexpr (internal::has_n_fields<T, 65>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65));
  } else if constexpr (internal::has_n_fields<T, 66>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66));
  } else if constexpr (internal::has_n_fields<T, 67>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67));
  } else if constexpr (internal::has_n_fields<T, 68>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68));
  } else if constexpr (internal::has_n_fields<T, 69>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69));
  } else if constexpr (internal::has_n_fields<T, 70>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70));
  } else if constexpr (internal::has_n_fields<T, 71>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70,
           f71] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71));
  } else if constexpr (internal::has_n_fields<T, 72>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72));
  } else if constexpr (internal::has_n_fields<T, 73>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73));
  } else if constexpr (internal::has_n_fields<T, 74>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74));
  } else if constexpr (internal::has_n_fields<T, 75>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75));
  } else if constexpr (internal::has_n_fields<T, 76>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76));
  } else if constexpr (internal::has_n_fields<T, 77>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77));
  } else if constexpr (internal::has_n_fields<T, 78>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78));
  } else if constexpr (internal::has_n_fields<T, 79>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79));
  } else if constexpr (internal::has_n_fields<T, 80>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80));
  } else if constexpr (internal::has_n_fields<T, 81>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81));
  } else if constexpr (internal::has_n_fields<T, 82>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82));
  } else if constexpr (internal::has_n_fields<T, 83>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83));
  } else if constexpr (internal::has_n_fields<T, 84>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84));
  } else if constexpr (internal::has_n_fields<T, 85>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84,
           f85] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85));
  } else if constexpr (internal::has_n_fields<T, 86>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86));
  } else if constexpr (internal::has_n_fields<T, 87>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87));
  } else if constexpr (internal::has_n_fields<T, 88>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88));
  } else if constexpr (internal::has_n_fields<T, 89>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89));
  } else if constexpr (internal::has_n_fields<T, 90>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90));
  } else if constexpr (internal::has_n_fields<T, 91>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91));
  } else if constexpr (internal::has_n_fields<T, 92>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92));
  } else if constexpr (internal::has_n_fields<T, 93>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93));
  } else if constexpr (internal::has_n_fields<T, 94>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94));
  } else if constexpr (internal::has_n_fields<T, 95>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95));
  } else if constexpr (internal::has_n_fields<T, 96>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95), std::move(f96));
  } else if constexpr (internal::has_n_fields<T, 97>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95), std::move(f96),
        std::move(f97));
  } else if constexpr (internal::has_n_fields<T, 98>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98] =
        _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95), std::move(f96),
        std::move(f97), std::move(f98));
  } else if constexpr (internal::has_n_fields<T, 99>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98,
           f99] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95), std::move(f96),
        std::move(f97), std::move(f98), std::move(f99));
  } else if constexpr (internal::has_n_fields<T, 100>) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98, f99,
           f100] = _t;
    return std::make_tuple(
        std::move(f1), std::move(f2), std::move(f3), std::move(f4),
        std::move(f5), std::move(f6), std::move(f7), std::move(f8),
        std::move(f9), std::move(f10), std::move(f11), std::move(f12),
        std::move(f13), std::move(f14), std::move(f15), std::move(f16),
        std::move(f17), std::move(f18), std::move(f19), std::move(f20),
        std::move(f21), std::move(f22), std::move(f23), std::move(f24),
        std::move(f25), std::move(f26), std::move(f27), std::move(f28),
        std::move(f29), std::move(f30), std::move(f31), std::move(f32),
        std::move(f33), std::move(f34), std::move(f35), std::move(f36),
        std::move(f37), std::move(f38), std::move(f39), std::move(f40),
        std::move(f41), std::move(f42), std::move(f43), std::move(f44),
        std::move(f45), std::move(f46), std::move(f47), std::move(f48),
        std::move(f49), std::move(f50), std::move(f51), std::move(f52),
        std::move(f53), std::move(f54), std::move(f55), std::move(f56),
        std::move(f57), std::move(f58), std::move(f59), std::move(f60),
        std::move(f61), std::move(f62), std::move(f63), std::move(f64),
        std::move(f65), std::move(f66), std::move(f67), std::move(f68),
        std::move(f69), std::move(f70), std::move(f71), std::move(f72),
        std::move(f73), std::move(f74), std::move(f75), std::move(f76),
        std::move(f77), std::move(f78), std::move(f79), std::move(f80),
        std::move(f81), std::move(f82), std::move(f83), std::move(f84),
        std::move(f85), std::move(f86), std::move(f87), std::move(f88),
        std::move(f89), std::move(f90), std::move(f91), std::move(f92),
        std::move(f93), std::move(f94), std::move(f95), std::move(f96),
        std::move(f97), std::move(f98), std::move(f99), std::move(f100));
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
