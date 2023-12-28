#ifndef RFL_INTERNAL_DEFINELITERAL_HPP_
#define RFL_INTERNAL_DEFINELITERAL_HPP_

#include "../Literal.hpp"

namespace rfl {
namespace internal {

/// Allows you to combine several literals.
template <class... LiteralTypes>
struct define_literal;

/// General case
template <StringLiteral... _content1, StringLiteral... _content2, class... Tail>
struct define_literal<Literal<_content1...>, Literal<_content2...>, Tail...> {
    using type = typename define_literal<Literal<_content1..., _content2...>,
                                         Tail...>::type;
};

/// Special case - only a single literal is left
template <StringLiteral... _content>
struct define_literal<Literal<_content...>> {
    using type = Literal<_content...>;
};

}  // namespace internal
}  // namespace rfl

#endif
