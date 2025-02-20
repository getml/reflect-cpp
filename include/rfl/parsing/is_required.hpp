#ifndef RFL_PARSING_IS_REQUIRED_HPP_
#define RFL_PARSING_IS_REQUIRED_HPP_

#include <memory>
#include <optional>
#include <type_traits>

#include "../Generic.hpp"
#include "../Rename.hpp"
#include "../internal/StringLiteral.hpp"
#include "../internal/is_rename.hpp"
#include "is_map_like.hpp"
#include "is_vector_like.hpp"

namespace rfl {
namespace parsing {

/// Determines whether a field in a named tuple is required.
/// General case - most fields are required.
template <class T>
class is_never_required;

template <class T>
class is_never_required : public std::false_type {};

template <class T>
class is_never_required<std::optional<T>> : public std::true_type {};

template <class T>
class is_never_required<std::shared_ptr<T>> : public std::true_type {};

template <class T>
class is_never_required<std::unique_ptr<T>> : public std::true_type {};

template <class T>
constexpr bool is_never_required_v = is_never_required<T>::value;

template <class T, bool _ignore_empty_containers>
consteval bool is_required() {
  using Type = std::remove_cvref_t<std::remove_pointer_t<T>>;
  if constexpr (internal::has_reflection_type_v<Type> &&
                !std::is_same_v<Type, rfl::Generic>) {
    return is_required<typename Type::ReflectionType,
                       _ignore_empty_containers>();
  } else if constexpr (internal::is_rename_v<Type>) {
    return is_required<typename Type::Type, _ignore_empty_containers>();
  } else {
    return !(is_never_required_v<Type> ||
             (_ignore_empty_containers &&
              (is_vector_like_v<Type> || is_map_like_v<Type>)));
  }
}

}  // namespace parsing
}  // namespace rfl

#endif
