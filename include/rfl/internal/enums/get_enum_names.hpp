#ifndef RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_GET_ENUM_NAMES_HPP_

#ifndef REFLECTCPP_USE_CPP26_REFLECTION
#include "cpp20/get_enum_names.hpp"
#else
#include "cpp26/get_enum_names.hpp"
#endif

namespace rfl::internal::enums {

template <class EnumType>
consteval auto get_enum_names() {
#ifndef REFLECTCPP_USE_CPP26_REFLECTION
  return cpp20::get_enum_names<EnumType>();
#else
  return cpp26::get_enum_names<EnumType>();
#endif
}

}  // namespace rfl::internal::enums

#endif
