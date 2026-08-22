#ifndef RFL_INTERNAL_ENUMS_CPP20_GET_ENUM_RANGE_HPP_
#define RFL_INTERNAL_ENUMS_CPP20_GET_ENUM_RANGE_HPP_

#include <utility>

// Necessary to use enchantum::enum_traits
#include "get_enum_names.hpp"

namespace rfl::internal::enums::cpp20 {

// Returns the range of the given enum type as a pair of the minimum and maximum
template <class EnumType>
constexpr auto get_enum_range() {
  return std::make_pair(enchantum::enum_traits<EnumType>::min,
                        enchantum::enum_traits<EnumType>::max);
}

}  // namespace rfl::internal::enums::cpp20

#endif
