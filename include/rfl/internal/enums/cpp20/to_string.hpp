#ifndef RFL_INTERNAL_ENUMS_CPP20_TO_STRING_HPP_
#define RFL_INTERNAL_ENUMS_CPP20_TO_STRING_HPP_

#ifndef REFLECTCPP_USE_CPP26_REFLECTION

#include <string>
#include <type_traits>

#include "enchantum.hpp"  // NOLINT(unused-includes)

namespace rfl::internal::enums::cpp20 {

template <class EnumType>
auto to_string(const EnumType e) {
  const auto s = enchantum::to_string(e);
  return s.empty()
             ? std::to_string(static_cast<std::underlying_type_t<EnumType>>(e))
             : std::string(s);
}

}  // namespace rfl::internal::enums::cpp20

#endif
#endif
