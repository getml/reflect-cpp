#ifndef RFL_INTERNAL_HASFIELDS_HPP_
#define RFL_INTERNAL_HASFIELDS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Tuple.hpp"
#include "all_fields.hpp"
#include "is_field.hpp"
#include "is_flatten_field.hpp"
#include "ptr_tuple_t.hpp"

namespace rfl {
namespace internal {

template <class TupleType>
constexpr bool all_fields_or_flatten() {
  const auto is_true_for_one =
      []<int _i>(std::integral_constant<int, _i>) -> bool {
    using T = std::remove_cvref_t<tuple_element_t<_i, TupleType>>;
    if constexpr (is_flatten_field_v<T>) {
      return all_fields_or_flatten<
          ptr_tuple_t<typename std::remove_pointer_t<T>::Type>>();
    } else {
      return is_field_v<T>;
    }
  };

  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return (true && ... && is_true_for_one(std::integral_constant<int, _is>{}));
  }
  (std::make_integer_sequence<int, rfl::tuple_size_v<TupleType>>());
}

template <class TupleType>
constexpr bool some_fields_or_flatten() {
  const auto is_true_for_one =
      []<int _i>(std::integral_constant<int, _i>) -> bool {
    using T = std::remove_cvref_t<tuple_element_t<_i, TupleType>>;
    if constexpr (is_flatten_field_v<T>) {
      return some_fields_or_flatten<
          ptr_tuple_t<typename std::remove_pointer_t<T>::Type>>();
    } else {
      return is_field_v<T>;
    }
  };

  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return (false || ... ||
            is_true_for_one(std::integral_constant<int, _is>{}));
  }
  (std::make_integer_sequence<int, rfl::tuple_size_v<TupleType>>());
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
