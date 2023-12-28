#ifndef RFL_VISIT_HPP_
#define RFL_VISIT_HPP_

#include <variant>

#include "Literal.hpp"
#include "TaggedUnion.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/VisitTree.hpp"
#include "internal/VisitorWrapper.hpp"

namespace rfl {

/// Implements the visitor pattern for Literals.
template <class Visitor, internal::StringLiteral... _fields, class... Args>
inline auto visit(const Visitor& _visitor, const Literal<_fields...> _literal,
                  const Args&... _args) {
    constexpr int size = sizeof...(_fields);
    using WrapperType = internal::VisitorWrapper<Visitor, _fields...>;
    const auto wrapper = WrapperType(&_visitor);
    return internal::VisitTree::visit<0, size, WrapperType>(
        wrapper, _literal.value(), _args...);
}

/// Implements the visitor pattern for TaggedUnions.
template <class Visitor, internal::StringLiteral _discriminator, class... Args>
inline auto visit(const Visitor& _visitor,
                  const TaggedUnion<_discriminator, Args...>& _tagged_union) {
    return std::visit(_visitor, _tagged_union.variant());
}

}  // namespace rfl

#endif  // RFL_VISIT_HPP_
