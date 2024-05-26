#ifndef RFL_INTERNAL_ISNOOPTIONALS_HPP_
#define RFL_INTERNAL_ISNOOPTIONALS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../NoOptionals.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_no_optionals;

template <class T>
class is_no_optionals : public std::false_type {};

template <>
class is_no_optionals<NoOptionals> : public std::true_type {};

template <class T>
constexpr bool is_no_optionals_v =
    is_no_optionals<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
