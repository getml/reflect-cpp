#ifndef RFL_INTERNAL_TO_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_TUPLE_HPP_

#include <tuple>

#include "rfl/internal/has_flatten_fields.hpp"
#include "rfl/internal/is_field.hpp"
#include "rfl/internal/to_unflattened_ptr_tuple.hpp"

namespace rfl {
namespace internal {

/// Generates a std::tuple that contains pointers to the original values in
/// the struct.
template <class PtrTuple, class... Args>
auto flatten_ptr_tuple(const PtrTuple& _ptr_tuple, Args&&... _args) {
  constexpr auto i = sizeof...(Args);
  if constexpr (i == std::tuple_size_v<std::decay_t<PtrTuple>>) {
    return std::tuple_cat(std::forward<Args>(_args)...);
  } else {
    using T = std::tuple_element_t<i, std::decay_t<PtrTuple>>;
    if constexpr (internal::is_flatten_field_v<T>) {
      const auto subtuple =
          to_unflattened_ptr_tuple(std::get<i>(_ptr_tuple)->get());
      return flatten_ptr_tuple(_ptr_tuple, std::forward<Args>(_args)...,
                               flatten_ptr_tuple(subtuple));
    } else {
      return flatten_ptr_tuple(_ptr_tuple, std::forward<Args>(_args)...,
                               std::make_tuple(std::get<i>(_ptr_tuple)));
    }
  }
}

template <class T>
auto to_ptr_tuple(const T& _t) {
  const auto ptr_tuple = to_unflattened_ptr_tuple(_t);
  if constexpr (!has_flatten_fields<std::decay_t<decltype(ptr_tuple)>>()) {
    return ptr_tuple;
  } else {
    return flatten_ptr_tuple(ptr_tuple);
  }
}

}  // namespace internal
}  // namespace rfl

#endif
