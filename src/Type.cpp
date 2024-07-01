#include "rfl/parsing/schema/Type.hpp"

namespace rfl::parsing::schema {

/// Requires a lot of template instantiation, so we do not want this to be
/// inlined.

Type::Type() : variant_() {}

Type::Type(const VariantType& _variant) : variant_(_variant) {}

Type::~Type() = default;

}  // namespace rfl::parsing::schema
