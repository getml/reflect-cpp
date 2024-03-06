#ifndef RFL_MAKENAMEDTUPLE_HPP_
#define RFL_MAKENAMEDTUPLE_HPP_

#include <type_traits>

#include "NamedTuple.hpp"

namespace rfl {

/// Convenience constructor that doesn't require you
/// to explitly define the field types.
template <class... FieldTypes>
inline auto make_named_tuple(FieldTypes&&... _args) {
  return NamedTuple<std::remove_cvref_t<FieldTypes>...>(
      std::forward<FieldTypes>(_args)...);
}

/// Convenience constructor that doesn't require you
/// to explitly define the field types.
template <class... FieldTypes>
inline auto make_named_tuple(const FieldTypes&... _args) {
  return NamedTuple<FieldTypes...>(_args...);
}

/// Explicit overload for creating empty named tuples.
inline auto make_named_tuple() { return NamedTuple<>(); }

}  // namespace rfl

#endif  // RFL_MAKENAMEDTUPLE_HPP_
