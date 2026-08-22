#ifndef RFL_INTERNAL_ENUMS_CPP26_GET_ENUM_RANGE_HPP_
#define RFL_INTERNAL_ENUMS_CPP26_GET_ENUM_RANGE_HPP_

#ifdef REFLECTCPP_USE_CPP26_REFLECTION

#include <limits>
#include <type_traits>
#include <utility>

namespace rfl::internal::enums::cpp26 {

// Returns the range of the given enum type as a pair of the minimum and maximum
template <class EnumType>
constexpr auto get_enum_range() {
  return std::make_pair(
      std::numeric_limits<std::underlying_type_t<EnumType>>::min(),
      std::numeric_limits<std::underlying_type_t<EnumType>>::max());
}

}  // namespace rfl::internal::enums::cpp26

#endif
#endif
