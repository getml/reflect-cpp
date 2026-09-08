#ifndef RFL_INTERNAL_ISSHORT_HPP_
#define RFL_INTERNAL_ISSHORT_HPP_

#include <type_traits>

#include "../Short.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_short;

template <class T>
class is_short : public std::false_type {};

template <StringLiteral _name, class Type>
class is_short<Short<_name, Type>> : public std::true_type {};

template <class T>
constexpr bool is_short_v =
    is_short<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
