#ifndef RFL_VISIT_HPP_
#define RFL_VISIT_HPP_

#include <variant>

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

template <class F, class... AlternativeTypes>
inline auto visit(F& _f, Variant<AlternativeTypes...>& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
inline auto visit(F& _f, Variant<AlternativeTypes...>&& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
inline auto visit(F& _f, const Variant<AlternativeTypes...>& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
inline auto visit(const F& _f, Variant<AlternativeTypes...>& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
inline auto visit(const F& _f, Variant<AlternativeTypes...>&& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
inline auto visit(const F& _f, const Variant<AlternativeTypes...>& _v) {
  return _v.visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(F& _f, TaggedUnion<_discriminator, Args...>& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(F& _f, TaggedUnion<_discriminator, Args...>&& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(F& _f,
                  const TaggedUnion<_discriminator, Args...>& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(const F& _f,
                  TaggedUnion<_discriminator, Args...>& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(const F& _f,
                  TaggedUnion<_discriminator, Args...>&& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, internal::StringLiteral _discriminator, class... Args>
inline auto visit(const F& _f,
                  const TaggedUnion<_discriminator, Args...>& _tagged_union) {
  return _tagged_union.variant().visit(_f);
}

template <class F, class Head, class... Tail>
inline auto visit(F& _f, Head& _head, Tail&... _tail) {
  const auto f_outer = [&](auto& _h) {
    const auto f_inner = [&](auto&... _t) { return _f(_h, _t...); };
    return visit(f_inner, _tail...);
  };
  return _head.visit(f_outer);
}

template <class F, class Head, class... Tail>
inline auto visit(const F& _f, Head& _head, Tail&... _tail) {
  const auto f_outer = [&](auto& _h) {
    const auto f_inner = [&](auto&... _t) { return _f(_h, _t...); };
    return visit(f_inner, _tail...);
  };
  return _head.visit(f_outer);
}

}  // namespace rfl

#endif  // RFL_VISIT_HPP_
