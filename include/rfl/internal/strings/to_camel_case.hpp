#ifndef RFL_INTERNAL_STRINGS_TOCAMELCASE_HPP_
#define RFL_INTERNAL_STRINGS_TOCAMELCASE_HPP_

#include <string>
#include <vector>

namespace rfl {
namespace internal {
namespace strings {

inline char to_lower(const char ch) {
  if (ch >= 'A' && ch <= 'Z') {
    return ch + ('a' - 'A');
  } else {
    return ch;
  }
}

inline char to_upper(const char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return ch + ('A' - 'a');
  } else {
    return ch;
  }
}

/// Splits a string alongside the delimiter
inline std::string to_camel_case(const std::string& _str) {
  std::string result;
  bool capitalize = false;
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
  if (result.size() > 0) {
    result[0] = to_lower(result[0]);
  }
  return result;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
