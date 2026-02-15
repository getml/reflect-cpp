#ifndef RFL_INTERNAL_ISPOSITIONAL_HPP_
#define RFL_INTERNAL_ISPOSITIONAL_HPP_

#include <type_traits>

#include "../Positional.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_positional;

template <class T>
class is_positional : public std::false_type {};

template <class Type>
class is_positional<Positional<Type>> : public std::true_type {};

template <class T>
constexpr bool is_positional_v =
    is_positional<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
