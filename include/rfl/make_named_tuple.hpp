#ifndef RFL_MAKENAMEDTUPLE_HPP_
#define RFL_MAKENAMEDTUPLE_HPP_

#include <type_traits>

#include "NamedTuple.hpp"

namespace rfl {

/// Convenience constructor that doesn't require you
/// to explitly defined the field types.
template <class... FieldTypes>
auto make_named_tuple(FieldTypes&&... _args) {
  return NamedTuple<std::remove_cvref_t<FieldTypes>...>(
      std::forward<FieldTypes>(_args)...);
}

/// Convenience constructor that doesn't require you
/// to explitly defined the field types.
template <class Head, class... Tail>
auto make_named_tuple(const Head& _h, const Tail&... _t) {
  return NamedTuple<Head, Tail...>(_h, _t...);
}

}  // namespace rfl

#endif  // RFL_MAKENAMEDTUPLE_HPP_
