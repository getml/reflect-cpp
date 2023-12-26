#ifndef RFL_INTERNAL_HASFIELDS_HPP_
#define RFL_INTERNAL_HASFIELDS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "all_fields.hpp"
#include "is_field.hpp"
#include "is_flatten_field.hpp"
#include "ptr_tuple_t.hpp"

namespace rfl {
namespace internal {

template <class TupleType, int _i = 0>
constexpr bool all_fields_or_flatten() {
  if constexpr (_i == std::tuple_size_v<TupleType>) {
    return true;
  } else {
    using T = std::remove_cvref_t<std::tuple_element_t<_i, TupleType>>;
    if constexpr (is_flatten_field_v<T>) {
      return all_fields_or_flatten<
                 ptr_tuple_t<typename std::remove_pointer_t<T>::Type>>() &&
             all_fields_or_flatten<TupleType, _i + 1>();
    } else {
      return is_field_v<T> && all_fields_or_flatten<TupleType, _i + 1>();
    }
  }
}

template <class TupleType, int _i = 0>
constexpr bool some_fields_or_flatten() {
  if constexpr (_i == std::tuple_size_v<TupleType>) {
    return false;
  } else {
    using T = std::remove_cvref_t<std::tuple_element_t<_i, TupleType>>;
    if constexpr (is_flatten_field_v<T>) {
      return some_fields_or_flatten<
                 ptr_tuple_t<typename std::remove_pointer_t<T>::Type>>() ||
             some_fields_or_flatten<TupleType, _i + 1>();
    } else {
      return is_field_v<T> || some_fields_or_flatten<TupleType, _i + 1>();
    }
  }
}

template <class T>
constexpr bool has_fields() {
  if constexpr (is_named_tuple_v<T>) {
    return true;
  } else {
    using TupleType = ptr_tuple_t<T>;
    if constexpr (some_fields_or_flatten<TupleType>()) {
      static_assert(
          all_fields_or_flatten<TupleType>(),
          "If some of your fields are annotated using rfl::Field<...>, "
          "then you must annotate all of your fields. "
          "Also, you cannot combine annotated and "
          "unannotated fields using rfl::Flatten<...>.");
      return true;
    } else {
      return false;
    }
  }
}

}  // namespace internal
}  // namespace rfl

#endif
