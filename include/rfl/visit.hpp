#ifndef RFL_VISIT_HPP_
#define RFL_VISIT_HPP_

#include <variant>

#include "rfl/Enum.hpp"
#include "rfl/Literal.hpp"
#include "rfl/TaggedUnion.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/internal/VisitTree.hpp"
#include "rfl/internal/VisitorWrapper.hpp"

namespace rfl {

/// Implements the visitor pattern for Enums.
template <class Visitor, class... Args>
inline auto visit(const Visitor& _visitor, const Enum<Args...>& _enum) {
    return std::visit(_visitor, _enum.variant_);
}

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
    return std::visit(_visitor, _tagged_union.variant_);
}

}  // namespace rfl

#endif  // RFL_VISIT_HPP_
