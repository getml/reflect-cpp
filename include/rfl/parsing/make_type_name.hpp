#ifndef RFL_PARSING_MAKETYPENAME_HPP_
#define RFL_PARSING_MAKETYPENAME_HPP_

#include "../type_name_t.hpp"
#include "is_tagged_union_wrapper.hpp"

namespace rfl {
namespace parsing {

inline std::string replace_non_alphanumeric(std::string _str) {
  for (auto& ch : _str) {
    ch = std::isalnum(ch) ? ch : '_';
  }
  return _str;
}

template <class U>
static std::string make_type_name() {
  if constexpr (is_tagged_union_wrapper_v<U>) {
    return replace_non_alphanumeric(type_name_t<typename U::Type>().str() +
                                    "__tagged");
  } else {
    return replace_non_alphanumeric(type_name_t<U>().str());
  }
}

}  // namespace parsing
}  // namespace rfl

#endif
