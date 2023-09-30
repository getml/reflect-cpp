#ifndef RFL_INTERNAL_HAS_N_FIELDS_HPP_
#define RFL_INTERNAL_HAS_N_FIELDS_HPP_

#include <cstddef>
#include <type_traits>

namespace rfl {
namespace internal {

template <typename T>
concept aggregate = std::is_aggregate_v<T>;

template <typename T, typename... Args>
concept constructable = aggregate<T> && requires {
    T{std::declval<Args>()...};
};

struct any {
    template <typename T>
    constexpr operator T() const noexcept;
};

/*The following boilerplate code was generated using a Python script:

def has_field_template(num: int) -> str:
    anys = "".join([", any"] * num)
    return f"""
    template <class T>
    constexpr bool has_{num}_fields = constructable<T{anys}>;
    """
code = "".join((has_field_template(i) for i in range(101)))

with open("generated_code.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)

*/

template <class T>
constexpr bool has_0_fields = constructable<T>;

template <class T>
constexpr bool has_1_fields = constructable<T, any>;

template <class T>
constexpr bool has_2_fields = constructable<T, any, any>;

template <class T>
constexpr bool has_3_fields = constructable<T, any, any, any>;

template <class T>
constexpr bool has_4_fields = constructable<T, any, any, any, any>;

template <class T>
constexpr bool has_5_fields = constructable<T, any, any, any, any, any>;

template <class T>
constexpr bool has_6_fields = constructable<T, any, any, any, any, any, any>;

template <class T>
constexpr bool has_7_fields =
    constructable<T, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_8_fields =
    constructable<T, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_9_fields =
    constructable<T, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_10_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_11_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_12_fields = constructable<T, any, any, any, any, any, any,
                                             any, any, any, any, any, any>;

template <class T>
constexpr bool has_13_fields = constructable<T, any, any, any, any, any, any,
                                             any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_14_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any>;

template <class T>
constexpr bool has_15_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_16_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_17_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_18_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_19_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_20_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_21_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_22_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_23_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_24_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_25_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any>;

template <class T>
constexpr bool has_26_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any>;

template <class T>
constexpr bool has_27_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_28_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_29_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_30_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_31_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_32_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_33_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_34_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_35_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_36_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_37_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any>;

template <class T>
constexpr bool has_38_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any>;

template <class T>
constexpr bool has_39_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_40_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_41_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_42_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_43_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_44_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_45_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_46_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_47_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_48_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_49_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any>;

template <class T>
constexpr bool has_50_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any>;

template <class T>
constexpr bool has_51_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_52_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_53_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_54_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_55_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_56_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_57_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_58_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_59_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_60_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_61_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any>;

template <class T>
constexpr bool has_62_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any>;

template <class T>
constexpr bool has_63_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_64_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_65_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_66_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_67_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_68_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_69_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_70_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_71_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_72_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_73_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_74_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_75_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any>;

template <class T>
constexpr bool has_76_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any>;

template <class T>
constexpr bool has_77_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any>;

template <class T>
constexpr bool has_78_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_79_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_80_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_81_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_82_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_83_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_84_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_85_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_86_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_87_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_88_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_89_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_90_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any>;

template <class T>
constexpr bool has_91_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_92_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_93_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_94_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_95_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_96_fields =
    constructable<T, any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any,
                  any, any, any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_97_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_98_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_99_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any>;

template <class T>
constexpr bool has_100_fields = constructable<
    T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
    any, any, any, any, any, any, any, any, any, any, any>;

}  // namespace internal
}  // namespace rfl

#endif
