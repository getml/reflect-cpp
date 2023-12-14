#ifndef RFL_INTERNAL_STRINGS_JOIN_HPP_
#define RFL_INTERNAL_STRINGS_JOIN_HPP_

#include <string>
#include <vector>

namespace rfl {
namespace internal {
namespace strings {

/// Joins a string using the delimiter
inline std::string join(const std::string& _delimiter,
                        const std::vector<std::string>& _strings) {
  if (_strings.size() == 0) {
    return "";
  }
  auto res = _strings[0];
  for (size_t i = 1; i < _strings.size(); ++i) {
    res += _delimiter + _strings[i];
  }
  return res;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
