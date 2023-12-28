#ifndef RFL_INTERNAL_VISITORWRAPPER_HPP_
#define RFL_INTERNAL_VISITORWRAPPER_HPP_

#include "../Literal.hpp"
#include "../TaggedUnion.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

/// Necessary for the VisitTree structure.
template <class Visitor, internal::StringLiteral... _fields>
struct VisitorWrapper {
    /// Calls the underlying visitor when required to do so.
    template <int _i, class... Args>
    inline auto visit(const Args&... _args) const {
        return (*visitor_)(name_of<Literal<_fields...>, _i>(), _args...);
    }

    /// The underlying visitor.
    const Visitor* visitor_;
};

}  // namespace internal
}  // namespace rfl

#endif  // RFL_VISIT_HPP_
