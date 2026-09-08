#ifndef RFL_VISIT_HPP_
#define RFL_VISIT_HPP_

#include <utility>

#include "Literal.hpp"
#include "TaggedUnion.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/VisitTree.hpp"
#include "internal/VisitorWrapper.hpp"

namespace rfl {

/// Visits a Literal with a visitor function.
/// @param _visitor The visitor to apply
/// @param _literal The literal to visit
/// @param _args Additional arguments to pass to the visitor
/// @return The result of the visitor function
template <class Visitor, internal::StringLiteral... _fields, class... Args>
inline auto visit(const Visitor& _visitor, const Literal<_fields...> _literal,
                  const Args&... _args) {
  constexpr int size = sizeof...(_fields);
  using WrapperType = internal::VisitorWrapper<Visitor, _fields...>;
  const auto wrapper = WrapperType{&_visitor};
  return internal::VisitTree::visit<0, size, WrapperType>(
      wrapper, _literal.value(), _args...);
}

/// Visits a variant-based type with a visitor function.
/// @param _f The visitor function to apply
/// @param _v The variant to visit
/// @return The result of the visitor function
template <class F, VariantBased V>
inline auto visit(F&& _f, V&& _v)
    -> decltype(std::declval<V&&>().visit(std::declval<F&&>())) {
  return std::forward<V>(_v).visit(std::forward<F>(_f));
}

/// Visits a tagged union with a visitor function.
/// @param _f The visitor function to apply
/// @param _tagged_union The tagged union to visit
/// @return The result of the visitor function
template <class F, TaggedUnionBased T>
inline auto visit(F&& _f, T&& _tagged_union)
    -> decltype(std::declval<T&&>().variant().visit(std::declval<F&&>())) {
  return std::forward<T>(_tagged_union).variant().visit(std::forward<F>(_f));
}

/// Visits multiple variants with a visitor function (Cartesian product).
/// @param _f The visitor function to apply
/// @param _head The first variant to visit
/// @param _tail The remaining variants to visit
/// @return The result of the visitor function applied to all combinations
template <class F, class Head, class... Tail>
inline auto visit(F&& _f, Head&& _head, Tail&&... _tail) {
  const auto f_outer = [&](auto& _h) {
    const auto f_inner = [&](auto&... _t) {
      return std::forward<F>(_f)(_h, _t...);
    };
    return visit(f_inner, std::forward<Tail>(_tail)...);
  };
  return std::forward<Head>(_head).visit(f_outer);
}

}  // namespace rfl

#endif  // RFL_VISIT_HPP_
