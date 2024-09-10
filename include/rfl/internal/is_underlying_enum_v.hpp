#ifndef RFL_INTERNAL_ISUNDERLYINGENUM_HPP_
#define RFL_INTERNAL_ISUNDERLYINGENUM_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../UnderlyingEnum.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_underlying_enum;

template <class T>
class is_underlying_enum : public std::false_type {};

template <>
class is_underlying_enum<UnderlyingEnum> : public std::true_type {};

template <class T>
constexpr bool is_underlying_enum_v =
    is_underlying_enum<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
