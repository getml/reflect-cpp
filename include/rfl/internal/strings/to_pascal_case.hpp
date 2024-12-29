#ifndef RFL_INTERNAL_STRINGS_TOPASCALCASE_HPP_
#define RFL_INTERNAL_STRINGS_TOPASCALCASE_HPP_

#include "to_camel_case.hpp"

namespace rfl {
namespace internal {
namespace strings {

/// Splits a string alongside the delimiter
inline std::string to_pascal_case(const std::string& _str) {
  auto result = to_camel_case("_" + _str);
  if (result.size() > 0) {
    result[0] = to_upper(result[0]);
  }
  return result;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
