#ifndef RFL_INTERNAL_TO_PTR_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_FIELD_TUPLE_HPP_

#include <iostream>
#include <tuple>
#include <type_traits>

#include "../field_names_t.hpp"
#include "bind_to_tuple.hpp"
#include "has_fields.hpp"
#include "is_empty.hpp"
#include "is_named_tuple.hpp"
#include "nt_to_ptr_named_tuple.hpp"
#include "to_ptr_field.hpp"
#include "wrap_in_fields.hpp"

namespace rfl {
namespace internal {

template <class T>
auto to_ptr_field_tuple(T& _t) {
  if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>) {
    return to_ptr_field_tuple(*_t);
  } else if constexpr (is_named_tuple_v<T>) {
    return nt_to_ptr_named_tuple(_t).fields();
  } else if constexpr (has_fields<T>()) {
    return bind_to_tuple(_t, [](auto* _ptr) { return to_ptr_field(*_ptr); });
  } else if constexpr (is_empty<T>()) {
    return rfl::Tuple();
  } else {
    using FieldNames = field_names_t<T>;
    auto tup =
        bind_to_tuple(_t, [](auto* _ptr) { return to_ptr_field(*_ptr); });
    return wrap_in_fields<FieldNames>(std::move(tup));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
