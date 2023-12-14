#ifndef RFL_INTERNAL_STRINGS_SPLIT_HPP_
#define RFL_INTERNAL_STRINGS_SPLIT_HPP_

#include <string>
#include <vector>

namespace rfl {
namespace internal {
namespace strings {

/// Splits a string alongside the delimiter
inline std::vector<std::string> split(const std::string& _str,
                                      const std::string& _delimiter) {
  auto str = _str;
  size_t pos = 0;
  std::vector<std::string> result;
  while ((pos = str.find(_delimiter)) != std::string::npos) {
    result.emplace_back(str.substr(0, pos));
    str.erase(0, pos + _delimiter.length());
  }
  result.emplace_back(std::move(str));
  return result;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
