#pragma once

// Give constants referenced by imported third-party declarations external
// linkage. Include only in the module interface, so there is exactly one
// definition.
#if defined(__GNUC__) && __GNUC__ == 15 && !defined(__clang__)
namespace YAML::ErrorMsg {
extern const char* const BAD_SUBSCRIPT;
}
#endif
