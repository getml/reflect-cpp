module;

#include "rfl/internal/ctre_std.hpp"

export module rfl.ctre;

// Use CTRE's native module exports. rfl imports this dependency privately.
#define CTRE_IN_A_MODULE
#define CTLL_IN_A_MODULE
#define UNICODE_DB_IN_A_MODULE
#if __has_include(<ctre.hpp>)
#include <ctre.hpp>
#else
#include "rfl/thirdparty/ctre.hpp"
#endif

export namespace rfl::internal {
template <auto Regex>
bool ctre_match(const std::string& value) {
  return static_cast<bool>(ctre::match<ctll::fixed_string<Regex.length>{
      ctll::construct_from_pointer_t{}, Regex.arr_.data()}>(value));
}
}
