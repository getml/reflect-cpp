#ifndef RFL_INTERNAL_TO_STD_ARRAY_HPP_
#define RFL_INTERNAL_TO_STD_ARRAY_HPP_

#include <cstdint>
#include <array>

namespace rfl::internal {

template <class T>
struct to_std_array {
    using type = T;
};

template <class T, std::size_t _n>
struct to_std_array<T[_n]> {
    using type = std::array<typename to_std_array<std::remove_cvref_t<T>>::type, _n>;
};

template <class T>
using to_std_array_t = to_std_array<T>::type;

}

#endif
