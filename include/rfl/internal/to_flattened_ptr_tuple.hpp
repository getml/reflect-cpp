#ifndef RFL_INTERNAL_TO_FLATTENED_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TO_FLATTENED_PTR_TUPLE_HPP_

#include <tuple>

#include "has_flatten_fields.hpp"
#include "is_flatten_field.hpp"
#include "to_ptr_tuple.hpp"

namespace rfl {
namespace internal {

template <class PtrTuple, class... Args>
auto flatten_ptr_tuple(PtrTuple&& _t, Args... _args) {
  constexpr auto i = sizeof...(Args);
  if constexpr (i == 0 && !has_flatten_fields<PtrTuple>()) {
    return std::forward<PtrTuple>(_t);
  } else if constexpr (i == std::tuple_size_v<std::remove_cvref_t<PtrTuple>>) {
    return std::tuple_cat(std::forward<Args>(_args)...);
  } else {
    using T = std::tuple_element_t<i, std::remove_cvref_t<PtrTuple>>;
    if constexpr (is_flatten_field_v<T>) {
      return flatten_ptr_tuple(
          std::forward<PtrTuple>(_t), std::forward<Args>(_args)...,
          flatten_ptr_tuple(to_ptr_tuple(std::get<i>(_t)->get())));
    } else {
      return flatten_ptr_tuple(std::forward<PtrTuple>(_t),
                               std::forward<Args>(_args)...,
                               std::make_tuple(std::get<i>(_t)));
    }
  }
}

template <class T>
auto to_flattened_ptr_tuple(T&& _t) {
  return flatten_ptr_tuple(to_ptr_tuple(_t));
}

}  // namespace internal
}  // namespace rfl

#endif
