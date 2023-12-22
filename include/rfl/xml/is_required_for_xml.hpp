#ifndef RFL_XML_IS_REQUIRED_FOR_XML_HPP_
#define RFL_XML_IS_REQUIRED_FOR_XML_HPP_

#include <type_traits>

#include "../internal/has_reflection_type_v.hpp"
#include "../parsing/is_required.hpp"
#include "../parsing/is_vector_like.hpp"

namespace rfl {
namespace xml {

// Empty vectors are simply not represented in XML. Therefore, XML requires more
// lax rules when things are required.
template <class T>
consteval bool is_required_for_xml() {
  using Type = std::decay_t<std::remove_pointer_t<T>>;
  if constexpr (internal::has_reflection_type_v<T>) {
    return is_required_for_xml<typename T::ReflectionType>();
  } else {
    return parsing::is_required<Type>() && !parsing::is_vector_like_v<Type>;
  }
}

}  // namespace xml
}  // namespace rfl

#endif
