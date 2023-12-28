#ifndef RFL_PATTERNS_HPP_
#define RFL_PATTERNS_HPP_

#include "Pattern.hpp"
#include "Validator.hpp"

namespace rfl {

/// This pattern only matches alphanumeric characters.
using AlphaNumeric = Pattern<R"(^[a-zA-Z0-9]*$)", "AlphaNumeric">;

/// This pattern will match valid Base64 encoded strings with or without
/// padding.
using Base64Encoded = Pattern<R"(^[A-Za-z0-9+/]*(={0,2})?$)", "Base64Encoded">;

/// This pattern will match email addresses.
using Email =
    Pattern<R"(^[a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,}$)", "Email">;

using UUIDv1 = Pattern<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-1[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)",
    "UUIDv1">;

using UUIDv2 = Pattern<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-2[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)",
    "UUIDv2">;

using UUIDv3 = Pattern<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-3[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)",
    "UUIDv3">;

using UUIDv4 = Pattern<
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)",
    "UUIDv4">;

}  // namespace rfl

#endif
