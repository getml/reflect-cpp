#ifndef RFL_INTERNAL_MAKE_TAG_HPP_
#define RFL_INTERNAL_MAKE_TAG_HPP_

#include "rfl/Literal.hpp"
#include "rfl/internal/get_type_name.hpp"
#include "rfl/internal/has_reflection_type_v.hpp"
#include "rfl/internal/has_tag_v.hpp"
#include "rfl/internal/remove_namespaces.hpp"

namespace rfl {
namespace internal {

template <class T>
static inline auto make_tag() noexcept {
  if constexpr (internal::has_reflection_type_v<T>) {
    return make_tag<typename T::ReflectionType>();
  } else if constexpr (internal::has_tag_v<T>) {
    using LiteralType = typename T::Tag;
    return LiteralType::template name_of<0>();
  } else {
    return rfl::Literal<
        internal::remove_namespaces<internal::get_type_name<T>()>()>();
  }
}

}  // namespace internal
}  // namespace rfl

#endif
