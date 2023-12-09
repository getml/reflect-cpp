#ifndef RFL_INTERNAL_TO_PTR_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_FIELD_TUPLE_HPP_

#include <iostream>
#include <tuple>
#include <type_traits>

#include "rfl/field_names_t.hpp"
#include "rfl/internal/bind_to_tuple.hpp"
#include "rfl/internal/has_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/nt_to_ptr_named_tuple.hpp"
#include "rfl/internal/to_ptr_field.hpp"
#include "rfl/internal/wrap_in_fields.hpp"

namespace rfl {
namespace internal {

template <class T>
auto to_ptr_field_tuple(T& _t) {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return to_ptr_field_tuple(*_t);
  } else if constexpr (is_named_tuple_v<T>) {
    return nt_to_ptr_named_tuple(_t).fields();
  } else if constexpr (has_fields<T>()) {
    return bind_to_tuple(_t, [](auto& x) { return to_ptr_field(x); });
  } else {
    using FieldNames = field_names_t<T>;
    auto tup = bind_to_tuple(_t, [](auto& x) { return to_ptr_field(x); });
    return wrap_in_fields<FieldNames>(std::move(tup));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
