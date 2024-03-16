#ifndef RFL_INTERNAL_ISARRAY_HPP_
#define RFL_INTERNAL_ISARRAY_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "Array.hpp"

namespace rfl::internal {

template <class T>
class is_array;

template <class T>
class is_array : public std::false_type {};

template <class Type>
class is_array<Array<Type>> : public std::true_type {};

template <class T>
constexpr bool is_array_v =
    is_array<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
