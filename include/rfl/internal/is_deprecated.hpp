#ifndef RFL_INTERNAL_ISDEPRECATED_HPP_
#define RFL_INTERNAL_ISDEPRECATED_HPP_

#include <type_traits>

#include "../Deprecated.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_deprecated;

template <class T>
class is_deprecated : public std::false_type {};

template <StringLiteral _message, StringLiteral _description, class Type>
class is_deprecated<Deprecated<_message, _description, Type>>
    : public std::true_type {};

template <class T>
constexpr bool is_deprecated_v =
    is_deprecated<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
