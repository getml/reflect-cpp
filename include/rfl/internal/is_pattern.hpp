#ifndef RFL_INTERNAL_ISPATTERN_HPP_
#define RFL_INTERNAL_ISPATTERN_HPP_

#include <type_traits>

#include "../Pattern.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_pattern;

template <class T>
class is_pattern : public std::false_type {};

template <StringLiteral _regex, internal::StringLiteral _name>
class is_pattern<Pattern<_regex, _name>> : public std::true_type {};

template <class T>
constexpr bool is_pattern_v =
    is_pattern<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
