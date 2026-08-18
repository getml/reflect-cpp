#ifndef RFL_INTERNAL_ENUMS_TO_STRING_OR_NUMBER_HPP_
#define RFL_INTERNAL_ENUMS_TO_STRING_OR_NUMBER_HPP_

#ifndef REFLECTCPP_USE_CPP26_REFLECTION
#include "cpp20/to_string.hpp"
#else
#include "cpp26/to_string.hpp"
#endif

namespace rfl::internal::enums {

template <class EnumType>
auto to_string(const EnumType e) {
#ifndef REFLECTCPP_USE_CPP26_REFLECTION
  return cpp20::to_string(e);
#else
  return cpp26::to_string(e);
#endif
}

}  // namespace rfl::internal::enums

#endif
