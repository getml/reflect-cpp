#ifndef RFL_INTERNAL_CONCATLITERALS_HPP_
#define RFL_INTERNAL_CONCATLITERALS_HPP_

#include "../Literal.hpp"
#include "StringLiteral.hpp"

namespace rfl::internal {

// We don't want the operator+ to apply to normal literals,
// so we introduce this wrapper.
template <StringLiteral... _names>
struct LiteralWrapper {
  Literal<_names...> literal_;
};

template <StringLiteral... _names>
consteval auto wrap_literal(const Literal<_names...>& _literal) {
  return LiteralWrapper<_names...>{_literal};
}

template <StringLiteral... _names1, StringLiteral... _names2>
consteval auto operator+(const LiteralWrapper<_names1...>&,
                         const LiteralWrapper<_names2...>&) {
  return LiteralWrapper<_names1..., _names2...>{
      rfl::Literal<_names1..., _names2...>()};
}

template <class Head, class... Tail>
consteval auto concat_literals(const Head& _head, const Tail&... _tail) {
  return (wrap_literal(_head) + ... + wrap_literal(_tail)).literal_;
}

// Special case - every literal contains exactly one value.
template <StringLiteral _head, StringLiteral... _tail>
consteval auto concat_literals(const rfl::Literal<_head>&,
                               const rfl::Literal<_tail>&...) {
  return rfl::Literal<_head, _tail...>();
}

// Special case - no literals.
consteval inline auto concat_literals() { return rfl::Literal<>(); }

}  // namespace rfl::internal

#endif
