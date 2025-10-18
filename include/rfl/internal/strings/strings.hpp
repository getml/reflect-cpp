#ifndef RFL_INTERNAL_STRINGS_STRINGS_HPP_
#define RFL_INTERNAL_STRINGS_STRINGS_HPP_

#include <string>
#include <vector>

#include "../../common.hpp"

namespace rfl::internal::strings {

/// Joins a series of strings.
RFL_API std::string join(const std::string& _delimiter,
                 const std::vector<std::string>& _strings);

/// Replace all occurences of _from with _to.
RFL_API std::string replace_all(const std::string& _str, const std::string& _from,
                        const std::string& _to);

/// Splits _str along _delimiter.
RFL_API std::vector<std::string> split(const std::string& _str,
                               const std::string& _delimiter);

/// Transforms the string to camel case.
RFL_API std::string to_camel_case(const std::string& _str);

/// Transforms the string to pascal case.
RFL_API std::string to_pascal_case(const std::string& _str);

}  // namespace rfl::internal::strings

#endif
