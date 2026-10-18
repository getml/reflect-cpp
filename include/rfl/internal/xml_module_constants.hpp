#pragma once

// Give constants referenced by imported third-party declarations external
// linkage. Include only in the module interface, so there is exactly one
// definition.
#if defined(__GNUC__) && __GNUC__ == 15 && !defined(__clang__)
namespace pugi {
extern const unsigned int format_default;
extern const unsigned int parse_default;
}  // namespace pugi
#endif
