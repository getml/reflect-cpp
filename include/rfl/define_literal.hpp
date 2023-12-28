#ifndef RFL_DEFINELITERAL_HPP_
#define RFL_DEFINELITERAL_HPP_

#include "Literal.hpp"
#include "internal/define_literal.hpp"

namespace rfl {

/// Allows you to combine several literal types.
template <class... LiteralTypes>
using define_literal_t =
    typename internal::define_literal<LiteralTypes...>::type;

}  // namespace rfl

#endif  // RFL_DEFINELITERAL_HPP_
