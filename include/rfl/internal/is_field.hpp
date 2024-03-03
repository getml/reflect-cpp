#ifndef RFL_INTERNAL_ISFIELD_HPP_
#define RFL_INTERNAL_ISFIELD_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Field.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_field;

template <class T>
class is_field : public std::false_type {};

template <StringLiteral _name, class Type>
class is_field<Field<_name, Type>> : public std::true_type {};

template <class T>
constexpr bool is_field_v =
    is_field<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
