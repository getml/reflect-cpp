#ifndef RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_

#include <tuple>
#include <type_traits>

#include "rfl/field_names_t.hpp"
#include "rfl/internal/bind_to_tuple.hpp"
#include "rfl/internal/has_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/wrap_in_fields.hpp"

namespace rfl {
namespace internal {

template <class OriginalStruct>
auto move_to_field_tuple(OriginalStruct&& _t) {
  using T = std::decay_t<OriginalStruct>;
  if constexpr (is_named_tuple_v<T>) {
    return _t.fields();
  } else if constexpr (has_fields<T>()) {
    return bind_to_tuple(_t, [](auto& x) { return std::move(x); });
  } else {
    using FieldNames = field_names_t<T>;
    auto tup = bind_to_tuple(_t, [](auto& x) { return std::move(x); });
    return wrap_in_fields<FieldNames>(std::move(tup));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
