#ifndef RFL_MAKENAMEDTUPLE_HPP_
#define RFL_MAKENAMEDTUPLE_HPP_

#include <type_traits>

#include "NamedTuple.hpp"

namespace rfl {

/// Convenience constructor for creating NamedTuples from rvalue Field arguments.
/// Creates a NamedTuple without requiring explicit template parameter specification.
/// The field types are automatically deduced from the arguments.
/// A NamedTuple is like a std::tuple but with compile-time string names for each field.
/// @tparam FieldTypes The Field types (automatically deduced)
/// @param _args The Field objects to include in the NamedTuple (rvalue references)
/// @return A NamedTuple containing the specified fields
template <class... FieldTypes>
inline auto make_named_tuple(FieldTypes&&... _args) {
  return NamedTuple<std::remove_cvref_t<FieldTypes>...>(
      std::forward<FieldTypes>(_args)...);
}

/// Convenience constructor for creating NamedTuples from const Field arguments.
/// Creates a NamedTuple without requiring explicit template parameter specification.
/// The field types are automatically deduced from the arguments.
/// A NamedTuple is like a std::tuple but with compile-time string names for each field.
/// @tparam FieldTypes The Field types (automatically deduced)
/// @param _args The Field objects to include in the NamedTuple (const references)
/// @return A NamedTuple containing the specified fields
template <class... FieldTypes>
inline auto make_named_tuple(const FieldTypes&... _args) {
  return NamedTuple<FieldTypes...>(_args...);
}

/// Creates an empty NamedTuple with no fields.
/// @return An empty NamedTuple<>
inline auto make_named_tuple() { return NamedTuple<>(); }

}  // namespace rfl

#endif  // RFL_MAKENAMEDTUPLE_HPP_
