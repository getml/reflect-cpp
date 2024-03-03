#ifndef RFL_INTERNAL_ISDESCRIPTION_HPP_
#define RFL_INTERNAL_ISDESCRIPTION_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Description.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_description;

template <class T>
class is_description : public std::false_type {};

template <StringLiteral _name, class Type>
class is_description<Description<_name, Type>> : public std::true_type {};

template <class T>
constexpr bool is_description_v =
    is_description<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
