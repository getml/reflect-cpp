#ifndef RFL_INTERNAL_GETFIELDNAMEPTRS_HPP_
#define RFL_INTERNAL_GETFIELDNAMEPTRS_HPP_

#include <array>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <type_traits>

#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/has_n_fields.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/is_rename.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

namespace rfl {
namespace internal {

/*The following boilerplate code was generated using a Python script:
def make_field_template(num: int) -> str:
    fields = ", ".join([f"f{i+1}" for i in range(num)])
    field_names = ", ".join([f"&f{i+1}" for i in range(num)])
    return (
        """
  } else if constexpr (_size == """
        + str(num)
        + """) {"""
        + ("auto& [" + fields + "] = fake_object<T>;" if num > 0 else "")
        + """
  return std::make_tuple("""
        + field_names
        + ");"
    )


code = "".join((make_field_template(i) for i in range(101)))

with open("generated_code5.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)
*/

/// Returns a tuple of pointers required to extract the field names.
template <class T, int _size>
constexpr auto get_field_name_ptrs() {
  if constexpr (_size == 0) {
    return std::make_tuple();
  } else if constexpr (_size == 1) {
    auto& [f1] = fake_object<T>;
    return std::make_tuple(&f1);
  } else if constexpr (_size == 2) {
    auto& [f1, f2] = fake_object<T>;
    return std::make_tuple(&f1, &f2);
  } else if constexpr (_size == 3) {
    auto& [f1, f2, f3] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3);
  } else if constexpr (_size == 4) {
    auto& [f1, f2, f3, f4] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4);
  } else if constexpr (_size == 5) {
    auto& [f1, f2, f3, f4, f5] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5);
  } else if constexpr (_size == 6) {
    auto& [f1, f2, f3, f4, f5, f6] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6);
  } else if constexpr (_size == 7) {
    auto& [f1, f2, f3, f4, f5, f6, f7] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7);
  } else if constexpr (_size == 8) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8);
  } else if constexpr (_size == 9) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9);
  } else if constexpr (_size == 10) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10);
  } else if constexpr (_size == 11) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11);
  } else if constexpr (_size == 12) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12);
  } else if constexpr (_size == 13) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13);
  } else if constexpr (_size == 14) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14);
  } else if constexpr (_size == 15) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15);
  } else if constexpr (_size == 16) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16);
  } else if constexpr (_size == 17) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17);
  } else if constexpr (_size == 18) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18);
  } else if constexpr (_size == 19) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18,
                           &f19);
  } else if constexpr (_size == 20) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20);
  } else if constexpr (_size == 21) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21);
  } else if constexpr (_size == 22) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22);
  } else if constexpr (_size == 23) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23);
  } else if constexpr (_size == 24) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24);
  } else if constexpr (_size == 25) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25);
  } else if constexpr (_size == 26) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26);
  } else if constexpr (_size == 27) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27);
  } else if constexpr (_size == 28) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27,
                           &f28);
  } else if constexpr (_size == 29) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
           f29] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29);
  } else if constexpr (_size == 30) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30);
  } else if constexpr (_size == 31) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31);
  } else if constexpr (_size == 32) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32);
  } else if constexpr (_size == 33) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33);
  } else if constexpr (_size == 34) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34);
  } else if constexpr (_size == 35) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35);
  } else if constexpr (_size == 36) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36);
  } else if constexpr (_size == 37) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37);
  } else if constexpr (_size == 38) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38);
  } else if constexpr (_size == 39) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39);
  } else if constexpr (_size == 40) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40);
  } else if constexpr (_size == 41) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41);
  } else if constexpr (_size == 42) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42);
  } else if constexpr (_size == 43) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42,
           f43] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43);
  } else if constexpr (_size == 44) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44);
  } else if constexpr (_size == 45) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45);
  } else if constexpr (_size == 46) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46);
  } else if constexpr (_size == 47) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47);
  } else if constexpr (_size == 48) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48);
  } else if constexpr (_size == 49) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49);
  } else if constexpr (_size == 50) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50);
  } else if constexpr (_size == 51) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51);
  } else if constexpr (_size == 52) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51, &f52);
  } else if constexpr (_size == 53) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51, &f52, &f53);
  } else if constexpr (_size == 54) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54] =
        fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51, &f52, &f53, &f54);
  } else if constexpr (_size == 55) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55);
  } else if constexpr (_size == 56) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56);
  } else if constexpr (_size == 57) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56,
           f57] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57);
  } else if constexpr (_size == 58) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58);
  } else if constexpr (_size == 59) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59);
  } else if constexpr (_size == 60) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60);
  } else if constexpr (_size == 61) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61);
  } else if constexpr (_size == 62) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51, &f52, &f53, &f54, &f55,
                           &f56, &f57, &f58, &f59, &f60, &f61, &f62);
  } else if constexpr (_size == 63) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63] = fake_object<T>;
    return std::make_tuple(&f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10,
                           &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19,
                           &f20, &f21, &f22, &f23, &f24, &f25, &f26, &f27, &f28,
                           &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
                           &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46,
                           &f47, &f48, &f49, &f50, &f51, &f52, &f53, &f54, &f55,
                           &f56, &f57, &f58, &f59, &f60, &f61, &f62, &f63);
  } else if constexpr (_size == 64) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64);
  } else if constexpr (_size == 65) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65);
  } else if constexpr (_size == 66) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66);
  } else if constexpr (_size == 67) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67);
  } else if constexpr (_size == 68) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68);
  } else if constexpr (_size == 69) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69);
  } else if constexpr (_size == 70) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70);
  } else if constexpr (_size == 71) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70,
           f71] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71);
  } else if constexpr (_size == 72) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72);
  } else if constexpr (_size == 73) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73);
  } else if constexpr (_size == 74) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74);
  } else if constexpr (_size == 75) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75);
  } else if constexpr (_size == 76) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76);
  } else if constexpr (_size == 77) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77);
  } else if constexpr (_size == 78) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78);
  } else if constexpr (_size == 79) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79);
  } else if constexpr (_size == 80) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80);
  } else if constexpr (_size == 81) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81);
  } else if constexpr (_size == 82) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82);
  } else if constexpr (_size == 83) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83);
  } else if constexpr (_size == 84) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84);
  } else if constexpr (_size == 85) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84,
           f85] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85);
  } else if constexpr (_size == 86) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86);
  } else if constexpr (_size == 87) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87);
  } else if constexpr (_size == 88) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88);
  } else if constexpr (_size == 89) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89);
  } else if constexpr (_size == 90) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90);
  } else if constexpr (_size == 91) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91);
  } else if constexpr (_size == 92) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92);
  } else if constexpr (_size == 93) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93);
  } else if constexpr (_size == 94) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94);
  } else if constexpr (_size == 95) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95);
  } else if constexpr (_size == 96) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95, &f96);
  } else if constexpr (_size == 97) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95, &f96, &f97);
  } else if constexpr (_size == 98) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98] =
        fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95, &f96, &f97,
        &f98);
  } else if constexpr (_size == 99) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98,
           f99] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95, &f96, &f97,
        &f98, &f99);
  } else if constexpr (_size == 100) {
    auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
           f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
           f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41, f42, f43,
           f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54, f55, f56, f57,
           f58, f59, f60, f61, f62, f63, f64, f65, f66, f67, f68, f69, f70, f71,
           f72, f73, f74, f75, f76, f77, f78, f79, f80, f81, f82, f83, f84, f85,
           f86, f87, f88, f89, f90, f91, f92, f93, f94, f95, f96, f97, f98, f99,
           f100] = fake_object<T>;
    return std::make_tuple(
        &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13,
        &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
        &f26, &f27, &f28, &f29, &f30, &f31, &f32, &f33, &f34, &f35, &f36, &f37,
        &f38, &f39, &f40, &f41, &f42, &f43, &f44, &f45, &f46, &f47, &f48, &f49,
        &f50, &f51, &f52, &f53, &f54, &f55, &f56, &f57, &f58, &f59, &f60, &f61,
        &f62, &f63, &f64, &f65, &f66, &f67, &f68, &f69, &f70, &f71, &f72, &f73,
        &f74, &f75, &f76, &f77, &f78, &f79, &f80, &f81, &f82, &f83, &f84, &f85,
        &f86, &f87, &f88, &f89, &f90, &f91, &f92, &f93, &f94, &f95, &f96, &f97,
        &f98, &f99, &f100);
  }
}

}  // namespace internal
}  // namespace rfl

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
