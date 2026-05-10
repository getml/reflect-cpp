#ifndef RFL_PARSING_MAKETYPENAME_HPP_
#define RFL_PARSING_MAKETYPENAME_HPP_

#include "../type_name_t.hpp"
#include "is_tagged_union_wrapper.hpp"

namespace rfl::parsing {

/**
 * @brief Replaces all non-alphanumeric characters with underscores.
 *
 * @param _str The string to process.
 * @return The processed string.
 */
inline std::string replace_non_alphanumeric(std::string _str) {
  for (auto& ch : _str) {
    ch = std::isalnum(ch) ? ch : '_';
  }
  return _str;
}

/**
 * @brief Generates a type name for the type U.
 *
 * @tparam U The type to generate the name for.
 * @return The type name.
 */
template <class U>
static std::string make_type_name() {
  if constexpr (is_tagged_union_wrapper_v<U>) {
    return replace_non_alphanumeric(type_name_t<typename U::Type>().str() +
                                    "__tagged");
  } else {
    return replace_non_alphanumeric(type_name_t<U>().str());
  }
}

}  // namespace rfl::parsing

#endif
