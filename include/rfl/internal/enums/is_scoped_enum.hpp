#ifndef RFL_INTERNAL_ENUMS_IS_SCOPED_ENUM_HPP_
#define RFL_INTERNAL_ENUMS_IS_SCOPED_ENUM_HPP_

#include <concepts>
#include <type_traits>
#include "../../thirdparty/enchantum.hpp"

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType>
concept is_scoped_enum = enchantum::ScopedEnum<EnumType>;

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
