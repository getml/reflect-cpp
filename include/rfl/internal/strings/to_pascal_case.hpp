#ifndef RFL_INTERNAL_STRINGS_TOPASCALCASE_HPP_
#define RFL_INTERNAL_STRINGS_TOPASCALCASE_HPP_

#include <string>
#include <vector>

namespace rfl {
namespace internal {
namespace strings {

inline char to_upper(const char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return ch + ('A' - 'a');
  } else {
    return ch;
  }
}

/// Splits a string alongside the delimiter
inline std::string to_pascal_case(const std::string& _str) {
  std::string result;
  bool capitalize = true;
  for (const char ch : _str) {
    if (ch == '_') {
      capitalize = true;
    } else if (capitalize) {
      result.push_back(to_upper(ch));
      capitalize = false;
    } else {
      result.push_back(ch);
    }
  }
  return result;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
