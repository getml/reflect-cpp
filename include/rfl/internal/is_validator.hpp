#ifndef RFL_INTERNAL_ISVALIDATOR_HPP_
#define RFL_INTERNAL_ISVALIDATOR_HPP_

#include <type_traits>

#include "../Validator.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_validator;

template <class T>
class is_validator : public std::false_type {};

template <class T, class V, class... Vs>
class is_validator<Validator<T, V, Vs...>> : public std::true_type {};

template <class T>
constexpr bool is_validator_v =
    is_validator<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
