#ifndef RFL_DEFINELITERAL_HPP_
#define RFL_DEFINELITERAL_HPP_

#include "Literal.hpp"
#include "internal/define_literal.hpp"

namespace rfl {

/// Combines several Literal types into a single union Literal type.
/// This is useful when you want to create a Literal that accepts values from multiple
/// different Literal types. For example, combining Literal<"A", "B"> and Literal<"C", "D">
/// results in Literal<"A", "B", "C", "D">.
/// @tparam LiteralTypes The Literal types to combine into a single union type
template <class... LiteralTypes>
using define_literal_t =
    typename internal::define_literal<LiteralTypes...>::type;

}  // namespace rfl

#endif  // RFL_DEFINELITERAL_HPP_
