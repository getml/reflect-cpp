#ifndef RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_
#define RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_

#include <concepts>

#include "is_scoped_enum.hpp"

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType>
concept is_flag_enum = is_scoped_enum<EnumType> &&
    requires(EnumType e1, EnumType e2) {
  { e1 | e2 } -> std::same_as<EnumType>;
};

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
