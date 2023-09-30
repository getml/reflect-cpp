#ifndef RFL_INTERNAL_TO_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_TO_FIELD_TUPLE_HPP_

#include <iostream>
#include <tuple>

#include "rfl/always_false.hpp"
#include "rfl/internal/has_n_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"

namespace rfl {
namespace internal {

/*The following boilerplate code was generated using a Python script:

def make_field_template(num: int) -> str:
    fields = ", ".join([f"f{i+1}" for i in range(num)])
    return (
        """
  } else if constexpr (internal::has_"""
        + str(num)
        + """_fields<T>) {
    auto& ["""
        + fields
        + """] = _t;
    return std::make_tuple("""
        + fields
        + ");"
    )


beginning = """
  if constexpr (internal::is_named_tuple_v<T>) {
    return _t.fields();
"""

main_part = "".join((make_field_template(i) for i in range(101)))

end = """} else {
    static_assert(rfl::always_false_v<T>, "Only up to 100 fields are
supported.");
}
"""

code = beginning + main_part + end

with open("generated_code2.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)
*/

template <class T>
auto to_field_tuple(const T& _t) {
    if constexpr (internal::is_named_tuple_v<T>) {
        return _t.fields();
    } else if constexpr (internal::has_0_fields<T>) {
        return std::make_tuple();
    } else if constexpr (internal::has_1_fields<T>) {
        auto& [f1] = _t;
        return std::make_tuple(f1);
    } else if constexpr (internal::has_2_fields<T>) {
        auto& [f1, f2] = _t;
        return std::make_tuple(f1, f2);
    } else if constexpr (internal::has_3_fields<T>) {
        auto& [f1, f2, f3] = _t;
        return std::make_tuple(f1, f2, f3);
    } else if constexpr (internal::has_4_fields<T>) {
        auto& [f1, f2, f3, f4] = _t;
        return std::make_tuple(f1, f2, f3, f4);
    } else if constexpr (internal::has_5_fields<T>) {
        auto& [f1, f2, f3, f4, f5] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5);
    } else if constexpr (internal::has_6_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6);
    } else if constexpr (internal::has_7_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7);
    } else if constexpr (internal::has_8_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8);
    } else if constexpr (internal::has_9_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9);
    } else if constexpr (internal::has_10_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10);
    } else if constexpr (internal::has_11_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);
    } else if constexpr (internal::has_12_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12);
    } else if constexpr (internal::has_13_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13);
    } else if constexpr (internal::has_14_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14] =
            _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14);
    } else if constexpr (internal::has_15_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14,
               f15] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15);
    } else if constexpr (internal::has_16_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16);
    } else if constexpr (internal::has_17_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17);
    } else if constexpr (internal::has_18_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18);
    } else if constexpr (internal::has_19_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19);
    } else if constexpr (internal::has_20_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20);
    } else if constexpr (internal::has_21_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20,
                               f21);
    } else if constexpr (internal::has_22_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22);
    } else if constexpr (internal::has_23_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23);
    } else if constexpr (internal::has_24_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24);
    } else if constexpr (internal::has_25_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25);
    } else if constexpr (internal::has_26_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26);
    } else if constexpr (internal::has_27_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27);
    } else if constexpr (internal::has_28_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27,
               f28] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28);
    } else if constexpr (internal::has_29_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29);
    } else if constexpr (internal::has_30_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30);
    } else if constexpr (internal::has_31_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30,
                               f31);
    } else if constexpr (internal::has_32_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32);
    } else if constexpr (internal::has_33_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33);
    } else if constexpr (internal::has_34_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34);
    } else if constexpr (internal::has_35_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35);
    } else if constexpr (internal::has_36_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36);
    } else if constexpr (internal::has_37_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37);
    } else if constexpr (internal::has_38_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38);
    } else if constexpr (internal::has_39_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39);
    } else if constexpr (internal::has_40_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40);
    } else if constexpr (internal::has_41_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40,
               f41] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41);
    } else if constexpr (internal::has_42_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42);
    } else if constexpr (internal::has_43_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43);
    } else if constexpr (internal::has_44_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44);
    } else if constexpr (internal::has_45_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45);
    } else if constexpr (internal::has_46_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46);
    } else if constexpr (internal::has_47_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47);
    } else if constexpr (internal::has_48_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48);
    } else if constexpr (internal::has_49_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49);
    } else if constexpr (internal::has_50_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50);
    } else if constexpr (internal::has_51_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51);
    } else if constexpr (internal::has_52_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52);
    } else if constexpr (internal::has_53_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53);
    } else if constexpr (internal::has_54_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53,
               f54] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54);
    } else if constexpr (internal::has_55_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55);
    } else if constexpr (internal::has_56_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56);
    } else if constexpr (internal::has_57_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57);
    } else if constexpr (internal::has_58_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58);
    } else if constexpr (internal::has_59_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58, f59);
    } else if constexpr (internal::has_60_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58, f59, f60);
    } else if constexpr (internal::has_61_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61);
    } else if constexpr (internal::has_62_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62);
    } else if constexpr (internal::has_63_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63);
    } else if constexpr (internal::has_64_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64);
    } else if constexpr (internal::has_65_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65);
    } else if constexpr (internal::has_66_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66);
    } else if constexpr (internal::has_67_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66,
               f67] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67);
    } else if constexpr (internal::has_68_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58, f59, f60, f61,
                               f62, f63, f64, f65, f66, f67, f68);
    } else if constexpr (internal::has_69_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58, f59, f60, f61,
                               f62, f63, f64, f65, f66, f67, f68, f69);
    } else if constexpr (internal::has_70_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70] = _t;
        return std::make_tuple(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
                               f12, f13, f14, f15, f16, f17, f18, f19, f20, f21,
                               f22, f23, f24, f25, f26, f27, f28, f29, f30, f31,
                               f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
                               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51,
                               f52, f53, f54, f55, f56, f57, f58, f59, f60, f61,
                               f62, f63, f64, f65, f66, f67, f68, f69, f70);
    } else if constexpr (internal::has_71_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71);
    } else if constexpr (internal::has_72_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72);
    } else if constexpr (internal::has_73_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73);
    } else if constexpr (internal::has_74_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74);
    } else if constexpr (internal::has_75_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75);
    } else if constexpr (internal::has_76_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76);
    } else if constexpr (internal::has_77_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77);
    } else if constexpr (internal::has_78_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78);
    } else if constexpr (internal::has_79_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79);
    } else if constexpr (internal::has_80_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79,
               f80] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80);
    } else if constexpr (internal::has_81_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81);
    } else if constexpr (internal::has_82_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82);
    } else if constexpr (internal::has_83_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83);
    } else if constexpr (internal::has_84_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84);
    } else if constexpr (internal::has_85_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85);
    } else if constexpr (internal::has_86_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86);
    } else if constexpr (internal::has_87_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87);
    } else if constexpr (internal::has_88_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88);
    } else if constexpr (internal::has_89_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89);
    } else if constexpr (internal::has_90_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90);
    } else if constexpr (internal::has_91_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91);
    } else if constexpr (internal::has_92_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92);
    } else if constexpr (internal::has_93_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92,
               f93] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93);
    } else if constexpr (internal::has_94_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94);
    } else if constexpr (internal::has_95_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95);
    } else if constexpr (internal::has_96_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95, f96] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95, f96);
    } else if constexpr (internal::has_97_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95, f96, f97] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95, f96, f97);
    } else if constexpr (internal::has_98_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95, f96, f97, f98] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95, f96, f97, f98);
    } else if constexpr (internal::has_99_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95, f96, f97, f98, f99] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95, f96, f97, f98, f99);
    } else if constexpr (internal::has_100_fields<T>) {
        auto& [f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
               f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
               f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
               f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
               f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
               f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
               f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
               f94, f95, f96, f97, f98, f99, f100] = _t;
        return std::make_tuple(
            f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15,
            f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28,
            f29, f30, f31, f32, f33, f34, f35, f36, f37, f38, f39, f40, f41,
            f42, f43, f44, f45, f46, f47, f48, f49, f50, f51, f52, f53, f54,
            f55, f56, f57, f58, f59, f60, f61, f62, f63, f64, f65, f66, f67,
            f68, f69, f70, f71, f72, f73, f74, f75, f76, f77, f78, f79, f80,
            f81, f82, f83, f84, f85, f86, f87, f88, f89, f90, f91, f92, f93,
            f94, f95, f96, f97, f98, f99, f100);
    } else {
        static_assert(rfl::always_false_v<T>,
                      "Only up to 100 fields are supported.");
    }
}

}  // namespace internal
}  // namespace rfl

#endif
