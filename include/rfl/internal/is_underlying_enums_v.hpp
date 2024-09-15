#ifndef RFL_INTERNAL_ISUNDERLYINGENUMS_HPP_
#define RFL_INTERNAL_ISUNDERLYINGENUMS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../UnderlyingEnums.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_underlying_enums;

template <class T>
class is_underlying_enums : public std::false_type {};

template <>
class is_underlying_enums<UnderlyingEnums> : public std::true_type {};

template <class T>
constexpr bool is_underlying_enums_v =
    is_underlying_enums<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
