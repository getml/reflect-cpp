#ifndef RFL_INTERNAL_ISLITERAL_HPP_
#define RFL_INTERNAL_ISLITERAL_HPP_

#include <type_traits>

#include "../Literal.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_literal;

template <class T>
class is_literal : public std::false_type {};

template <StringLiteral... _s>
class is_literal<Literal<_s...>> : public std::true_type {};

template <class T>
constexpr bool is_literal_v =
    is_literal<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
