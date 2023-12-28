#ifndef RFL_PATTERNVALIDATOR_HPP_
#define RFL_PATTERNVALIDATOR_HPP_

#include <string>

#include "../ctre.hpp"
#include "Result.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

template <internal::StringLiteral _regex, internal::StringLiteral _name>
struct PatternValidator {
  static Result<std::string> validate(const std::string& _str) noexcept {
    if (ctre::match<_regex.arr_>(_str))
      return _str;
    else
      return rfl::Error("String '" + _str + "' did not match format '" +
                        _name.str() + "': '" + _regex.str() + "'.");
  }
};

}  // namespace rfl

#endif
