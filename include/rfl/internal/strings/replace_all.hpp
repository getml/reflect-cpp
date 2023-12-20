#ifndef RFL_INTERNAL_STRINGS_REPLACE_ALL_HPP_
#define RFL_INTERNAL_STRINGS_REPLACE_ALL_HPP_

#include <string>
#include <vector>

namespace rfl {
namespace internal {
namespace strings {

inline std::string replace_all(const std::string& _str,
                               const std::string& _from,
                               const std::string& _to) {
  auto str = _str;

  size_t pos = 0;
  while ((pos = str.find(_from, pos)) != std::string::npos) {
    str.replace(pos, _from.length(), _to);
    pos += _to.length();
  }
  return str;
}

}  // namespace strings
}  // namespace internal
}  // namespace rfl

#endif
