#ifndef RFL_PATTERNVALIDATOR_HPP_
#define RFL_PATTERNVALIDATOR_HPP_

#include <ctime>
#include <iterator>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>

#include "internal/Memoization.hpp"
#include "internal/StringLiteral.hpp"
#include "rfl/Literal.hpp"
#include "rfl/Result.hpp"

namespace rfl {

template <internal::StringLiteral _regex, internal::StringLiteral _name>
struct PatternValidator {
  static Result<std::string> validate(const std::string& _str) noexcept {
    const auto check_pattern =
        [&](const auto _regex_pattern) -> rfl::Result<std::string> {
      if (!std::regex_match(_str, *_regex_pattern)) {
        return rfl::Error("String '" + _str + "' did not match format '" +
                          _name.str() + "': '" + _regex.str() + "'.");
      }
      return _str;
    };

    return regex_pattern().and_then(check_pattern);
  }

 private:
  /// Uses a memoization pattern to init the pattern.
  static rfl::Result<const std::regex*> regex_pattern() noexcept {
    const auto compile_pattern = [](std::optional<std::regex>* _regex_pattern) {
      try {
        *_regex_pattern = std::regex(_regex.str());
      } catch (std::exception& e) {
        return;
      }
    };
    const auto& r = regex_pattern_.value(compile_pattern);
    if (!r) {
      return Error("Regex pattern '" + _regex.str() + "' failed to compile.");
    }
    return &(*r);
  }

 private:
  /// The underlying regex pattern.
  static inline internal::Memoization<std::optional<std::regex>> regex_pattern_;
};

}  // namespace rfl

#endif
