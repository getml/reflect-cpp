#ifndef RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_

#include <tuple>
#include <type_traits>

#include "../Tuple.hpp"
#include "../field_names_t.hpp"
#include "Array.hpp"
#include "bind_to_tuple.hpp"
#include "has_fields.hpp"
#include "is_empty.hpp"
#include "is_named_tuple.hpp"
#include "wrap_in_fields.hpp"

namespace rfl {
namespace internal {

template <class OriginalStruct>
auto move_to_field_tuple(OriginalStruct&& _t) {
  using T = std::remove_cvref_t<OriginalStruct>;
  if constexpr (is_named_tuple_v<T>) {
    return _t.fields();
  } else if constexpr (has_fields<T>()) {
    return bind_to_tuple(_t, [](auto _ptr) { return std::move(*_ptr); });
  } else if constexpr (is_empty<T>()) {
    return rfl::Tuple();
  } else {
    using FieldNames = field_names_t<T>;
    const auto fct = []<class T>(T* _ptr) {
      using Type = std::remove_cvref_t<T>;
      if constexpr (std::is_array_v<Type>) {
        return Array<Type>(*_ptr);
      } else {
        return std::move(*_ptr);
      }
    };
    auto tup = bind_to_tuple(_t, fct);
    return wrap_in_fields<FieldNames>(std::move(tup));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
