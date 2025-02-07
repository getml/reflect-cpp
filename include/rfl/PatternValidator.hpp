#ifndef RFL_PATTERNVALIDATOR_HPP_
#define RFL_PATTERNVALIDATOR_HPP_

#include <sstream>
#include <string>

#if __has_include(<ctre.hpp>)
#include <ctre.hpp>
#else
#include "thirdparty/ctre.hpp"
#endif

#include "Literal.hpp"
#include "Result.hpp"
#include "internal/StringLiteral.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

template <internal::StringLiteral _regex, internal::StringLiteral _name>
struct PatternValidator {
  using Name = Literal<_name>;
  using Regex = Literal<_regex>;

  static Result<std::string> validate(const std::string& _str) noexcept {
    if (ctre::match<_regex.arr_>(_str)) {
      return _str;
    } else {
      std::stringstream stream;
      stream << "String '" << _str << "' did not match format '" << _name.str()
             << "': '" << _regex.str() << "'.";
      return error(stream.str());
    }
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    return ValidationType{ValidationType::Regex{.pattern_ = Regex().str()}};
  }
};

}  // namespace rfl

#endif
