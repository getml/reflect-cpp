#ifndef RFL_VISIT_HPP_
#define RFL_VISIT_HPP_

#include <utility>

#include "Literal.hpp"
#include "TaggedUnion.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/VisitTree.hpp"
#include "internal/VisitorWrapper.hpp"

namespace rfl {

template <class Visitor, internal::StringLiteral... _fields, class... Args>
inline auto visit(const Visitor& _visitor, const Literal<_fields...> _literal,
                  const Args&... _args) {
  constexpr int size = sizeof...(_fields);
  using WrapperType = internal::VisitorWrapper<Visitor, _fields...>;
  const auto wrapper = WrapperType{&_visitor};
  return internal::VisitTree::visit<0, size, WrapperType>(
      wrapper, _literal.value(), _args...);
}

template <class F, VariantBased V>
inline auto visit(F&& _f, V&& _v)
    -> decltype(std::declval<V&&>().visit(std::declval<F&&>())) {
  return std::forward<V>(_v).visit(std::forward<F>(_f));
}

template <class F, TaggedUnionBased T>
inline auto visit(F&& _f, T&& _tagged_union)
    -> decltype(std::declval<T&&>().variant().visit(std::declval<F&&>())) {
  return std::forward<T>(_tagged_union).variant().visit(std::forward<F>(_f));
}

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
