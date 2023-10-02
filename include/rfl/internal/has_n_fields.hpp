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

template <class T, int n>
struct constructable_with_n_fields;

/*The following boilerplate code was generated using a Python script:

def has_field_template(num: int) -> str:
    anys = "".join([", any"] * num)
    return """
    template <class T>
    struct constructable_with_n_fields<T, {num}> {
        static constexpr bool value = constructable<T{anys}>;
    };
    """.replace(
        "{num}", str(num)
    ).replace(
        "{anys}", anys
    )


code = "".join((has_field_template(i) for i in range(102)))
with open("generated_code.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(code)

*/

template <class T>
struct constructable_with_n_fields<T, 0> {
    static constexpr bool value = constructable<T>;
};

template <class T>
struct constructable_with_n_fields<T, 1> {
    static constexpr bool value = constructable<T, any>;
};

template <class T>
struct constructable_with_n_fields<T, 2> {
    static constexpr bool value = constructable<T, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 3> {
    static constexpr bool value = constructable<T, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 4> {
    static constexpr bool value = constructable<T, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 5> {
    static constexpr bool value = constructable<T, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 6> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 7> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 8> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 9> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 10> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 11> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 12> {
    static constexpr bool value = constructable<T, any, any, any, any, any, any,
                                                any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 13> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 14> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 15> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 16> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 17> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 18> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 19> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 20> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 21> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 22> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 23> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any>;
};

template <class T>
struct constructable_with_n_fields<T, 24> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 25> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 26> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 27> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 28> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 29> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 30> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 31> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 32> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 33> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 34> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any>;
};

template <class T>
struct constructable_with_n_fields<T, 35> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 36> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 37> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 38> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 39> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 40> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 41> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 42> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 43> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 44> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 45> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any>;
};

template <class T>
struct constructable_with_n_fields<T, 46> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 47> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 48> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 49> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 50> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 51> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 52> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 53> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 54> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 55> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 56> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 57> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 58> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 59> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 60> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 61> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 62> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 63> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 64> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 65> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 66> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 67> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 68> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 69> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 70> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 71> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 72> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 73> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 74> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 75> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 76> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 77> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 78> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 79> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 80> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 81> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 82> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 83> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 84> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 85> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 86> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 87> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 88> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 89> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 90> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 91> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 92> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 93> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 94> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 95> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 96> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 97> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 98> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 99> {
    static constexpr bool value =
        constructable<T, any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any,
                      any, any, any, any, any, any, any, any, any, any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 100> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any>;
};

template <class T>
struct constructable_with_n_fields<T, 101> {
    static constexpr bool value = constructable<
        T, any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any, any, any, any, any, any, any, any, any, any, any, any,
        any, any, any>;
};

// ---------------------------------
// End of generated boilerplate code
// ---------------------------------

template <class T, int n>
constexpr bool has_n_fields = constructable_with_n_fields<T, n>::value &&
                              !constructable_with_n_fields<T, n + 1>::value;

}  // namespace internal
}  // namespace rfl

#endif
