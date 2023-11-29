#ifndef RFL_PARSING_IS_REQUIRED_HPP_
#define RFL_PARSING_IS_REQUIRED_HPP_

#include <memory>
#include <optional>
#include <type_traits>

#include "rfl/Rename.hpp"
#include "rfl/internal/StringLiteral.hpp"

namespace rfl {
namespace parsing {

/// Determines whether a field in a named tuple is required.
/// General case - most fields are required.
template <class T>
class is_required;

template <class T>
class is_required : public std::true_type {};

template <class T>
class is_required<T*> : public std::false_type {};

template <class T>
class is_required<std::optional<T>> : public std::false_type {};

template <class T>
class is_required<std::shared_ptr<T>> : public std::false_type {};

template <class T>
class is_required<std::unique_ptr<T>> : public std::false_type {};

template <class T, internal::StringLiteral _name>
class is_required<Rename<_name, T>> : public is_required<T> {};

}  // namespace parsing
}  // namespace rfl

#endif
