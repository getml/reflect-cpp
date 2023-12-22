#ifndef RFL_PATTERN_HPP_
#define RFL_PATTERN_HPP_

#include "PatternValidator.hpp"
#include "Validator.hpp"

namespace rfl {

template <internal::StringLiteral _regex, internal::StringLiteral _name>
using Pattern = Validator<std::string, PatternValidator<_regex, _name>>;

}  // namespace rfl

#endif
