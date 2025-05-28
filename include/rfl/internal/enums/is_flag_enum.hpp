#ifndef RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_
#define RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_

#include <concepts>

#include "../../thirdparty/enchantum.hpp"
template<enchantum::Enum E>
requires requires(E e) {
  { e | e } -> std::same_as<E>;
}
constexpr inline bool enchantum::is_bitflag<E> = true;

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType>
concept is_flag_enum =
    enchantum::ScopedEnum<EnumType> && enchantum::BitFlagEnum<EnumType>;


}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
