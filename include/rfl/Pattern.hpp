#ifndef RFL_PATTERN_HPP_
#define RFL_PATTERN_HPP_

#include "PatternValidator.hpp"
#include "Validator.hpp"

namespace rfl {

/// Type alias for a string that must match a compile-time regex pattern.
/// Provides a convenient way to create validated strings with pattern constraints.
/// @tparam _regex The compile-time regular expression pattern to validate against
/// @tparam _name A human-readable name for the pattern (used in error messages)
template <internal::StringLiteral _regex, internal::StringLiteral _name>
using Pattern = Validator<std::string, PatternValidator<_regex, _name>>;

}  // namespace rfl

#endif
