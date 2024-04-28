#ifndef RFL_INTERNAL_TO_PTR_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_NAMED_TUPLE_HPP_

#include <iostream>
#include <tuple>

#include "../always_false.hpp"
#include "../field_names_t.hpp"
#include "../make_named_tuple.hpp"
#include "copy_flattened_tuple_to_named_tuple.hpp"
#include "has_fields.hpp"
#include "has_flatten_fields.hpp"
#include "is_empty.hpp"
#include "is_field.hpp"
#include "is_named_tuple.hpp"
#include "to_flattened_ptr_tuple.hpp"
#include "to_ptr_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class PtrFieldTuple, class... Args>
auto flatten_ptr_field_tuple(PtrFieldTuple& _t, Args&&... _args) {
  constexpr auto i = sizeof...(Args);
  if constexpr (i == std::tuple_size_v<std::remove_cvref_t<PtrFieldTuple>>) {
    return std::tuple_cat(std::forward<Args>(_args)...);
  } else {
    using T = std::tuple_element_t<i, std::remove_cvref_t<PtrFieldTuple>>;
    if constexpr (internal::is_flatten_field<T>::value) {
      auto subtuple = internal::to_ptr_field_tuple(*std::get<i>(_t).get());
      return flatten_ptr_field_tuple(_t, std::forward<Args>(_args)...,
                                     flatten_ptr_field_tuple(subtuple));
    } else {
      return flatten_ptr_field_tuple(_t, std::forward<Args>(_args)...,
                                     std::make_tuple(std::get<i>(_t)));
    }
  }
}

template <class PtrFieldTuple>
auto field_tuple_to_named_tuple(PtrFieldTuple& _ptr_field_tuple) {
  const auto ft_to_nt = []<class... Fields>(Fields&&... _fields) {
    return make_named_tuple(_fields...);
  };

  if constexpr (!has_flatten_fields<std::remove_cvref_t<PtrFieldTuple>>()) {
    return std::apply(ft_to_nt, std::move(_ptr_field_tuple));
  } else {
    const auto flattened_tuple = flatten_ptr_field_tuple(_ptr_field_tuple);
    return std::apply(ft_to_nt, flattened_tuple);
  }
}

/// Generates a named tuple that contains pointers to the original values in
/// the struct.
template <class T>
auto to_ptr_named_tuple(T&& _t) {
  if constexpr (has_fields<std::remove_cvref_t<T>>()) {
    if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>) {
      return to_ptr_named_tuple(*_t);
    } else if constexpr (is_named_tuple_v<std::remove_cvref_t<T>>) {
      return nt_to_ptr_named_tuple(_t);
    } else {
      auto ptr_field_tuple = to_ptr_field_tuple(_t);
      return field_tuple_to_named_tuple(ptr_field_tuple);
    }
  } else if constexpr (is_empty<T>()) {
    return rfl::NamedTuple<>();
  } else {
    using FieldNames = rfl::field_names_t<T>;
    auto flattened_ptr_tuple = to_flattened_ptr_tuple(_t);
    return copy_flattened_tuple_to_named_tuple<FieldNames>(flattened_ptr_tuple);
  }
}

}  // namespace internal
}  // namespace rfl

#endif
