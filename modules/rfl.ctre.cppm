module;

#include "rfl/internal/ctre_std.hpp"

export module rfl.ctre;

// GCC 15 loses CTRE's non-exported ADL overloads when instantiating an imported
// template. rfl imports this module privately, so these names stay out of rfl's API.
#if defined(__GNUC__) && __GNUC__ == 15 && !defined(__clang__)
export {
#endif
#if __has_include(<ctre.hpp>)
#include <ctre.hpp>
#else
#include "rfl/thirdparty/ctre.hpp"
#endif
#if defined(__GNUC__) && __GNUC__ == 15 && !defined(__clang__)
}
#endif

export namespace rfl::internal {
template <auto Regex>
bool ctre_match(const std::string& value) {
  return static_cast<bool>(ctre::match<ctll::fixed_string<Regex.length>{
      ctll::construct_from_pointer_t{}, Regex.arr_.data()}>(value));
}
}
