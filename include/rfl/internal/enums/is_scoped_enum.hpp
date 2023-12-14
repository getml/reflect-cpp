#ifndef RFL_INTERNAL_ENUMS_IS_SCOPED_ENUM_HPP_
#define RFL_INTERNAL_ENUMS_IS_SCOPED_ENUM_HPP_

#include <concepts>
#include <type_traits>

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType>
concept is_scoped_enum = std::is_enum_v<EnumType> &&
    !std::is_convertible_v<EnumType, std::underlying_type_t<EnumType>>;

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
