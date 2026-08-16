#ifndef RFL_INTERNAL_ENUMS_CPP20_FROM_STRING_HPP_
#define RFL_INTERNAL_ENUMS_CPP20_FROM_STRING_HPP_

#include <string>
#include <type_traits>

#include "../../../thirdparty/enchantum/enchantum.hpp"  // NOLINT(unused-includes)

namespace rfl::internal::enums::cpp20 {

template <class EnumType>
auto from_string(const std::string& _str) {
  return enchantum::cast<EnumType>(_str);
}

}  // namespace rfl::internal::enums::cpp20

#endif
