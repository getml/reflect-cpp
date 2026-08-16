#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_RANGE_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_RANGE_HPP_

#ifndef REFLECTCPP_USE_CPP_REFLECTION
#include "cpp20/get_enum_range.hpp"
#else
#include "cpp26/get_enum_range.hpp"
#endif

namespace rfl::internal::enums {

template <class EnumType>
constexpr auto get_enum_range() {
#ifndef REFLECTCPP_USE_CPP_REFLECTION
  return cpp20::get_enum_range<EnumType>();
#else
  return cpp26::get_enum_range<EnumType>();
#endif
}

}  // namespace rfl::internal::enums

#endif
