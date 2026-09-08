#ifndef RFL_ALWAYSFALSE_HPP_
#define RFL_ALWAYSFALSE_HPP_

namespace rfl {

/// Helper for static_assert with dependent types in templates.
/// This is useful in constexpr if branches to ensure compilation fails
/// when an unexpected branch is taken. Always evaluates to false but
/// depends on the template parameter so compilation is delayed.
/// @tparam T The template parameter (used to make the value dependent)
template <class>
inline constexpr bool always_false_v = false;

}  // namespace rfl

#endif  // RFL_ALWAYSFALSE_HPP_
