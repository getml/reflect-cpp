#ifndef RFL_INTERNAL_MAKE_TAG_HPP_
#define RFL_INTERNAL_MAKE_TAG_HPP_

#include "../Literal.hpp"
#include "../field_names_t.hpp"
#include "../named_tuple_t.hpp"
#include "../to_view.hpp"
#include "StringLiteral.hpp"
#include "get_type_name.hpp"
#include "has_reflection_type_v.hpp"
#include "has_tag_v.hpp"
#include "remove_namespaces.hpp"

namespace rfl::internal {

template <internal::StringLiteral _discriminator, class T>
static inline auto make_tag(const T& _t) noexcept {
  if constexpr (internal::has_reflection_type_v<T>) {
    return make_tag<_discriminator>(_t.reflection());
  } else if constexpr (named_tuple_t<T>::Names::template contains<
                           _discriminator>()) {
    return *to_view(_t).template get<_discriminator>();
  } else if constexpr (internal::has_tag_v<T>) {
    using LiteralType = typename T::Tag;
    return LiteralType::template name_of<0>();
  } else {
    return rfl::Literal<
        internal::remove_namespaces<internal::get_type_name<T>()>()>();
  }
}

}  // namespace rfl::internal

#endif
