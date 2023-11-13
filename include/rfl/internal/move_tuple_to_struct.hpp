#ifndef RFL_INTERNAL_MOVE_TUPLE_TO_STRUCT_HPP_
#define RFL_INTERNAL_MOVE_TUPLE_TO_STRUCT_HPP_

#include <tuple>
#include <type_traits>

#include "rfl/internal/has_flatten_field.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/ptr_tuple_t.hpp"

namespace rfl {
namespace internal {

template <class Tuple, int _i = 0>
constexpr int calc_flattened_size() {
  if constexpr (_i == std::tuple_size_v<Tuple>) {
    return 0;
  } else {
    using T = std::remove_pointer_t<std::tuple_element_t<_i, Tuple>>;
    if constexpr (is_flatten_field_v<T>) {
      return calc_flattened_size<ptr_tuple_t<typename T::Type>>() +
             calc_flattened_size<Tuple, _i + 1>();
    } else {
      return 1 + calc_flattened_size<Tuple, _i + 1>();
    }
  }
}

template <class ResultType, class Tuple, int _j = 0, class... Args>
ResultType move_tuple_to_struct(Tuple& _t, Args&&... _args) {
  using PtrTuple = ptr_tuple_t<ResultType>;
  constexpr auto i = sizeof...(Args);
  if constexpr (i == std::tuple_size_v<PtrTuple>) {
    return ResultType{std::move(_args)...};
  } else {
    using T = std::remove_pointer_t<std::tuple_element_t<i, PtrTuple>>;
    if constexpr (is_flatten_field_v<T>) {
      constexpr int flattened_size =
          calc_flattened_size<ptr_tuple_t<typename T::Type>>();
      return move_tuple_to_struct<ResultType, Tuple, _j + flattened_size>(
          _t, std::forward<Args>(_args)...,
          move_tuple_to_struct<typename T::Type, Tuple, _j>(_t));
    } else {
      return move_tuple_to_struct<ResultType, Tuple, _j + 1>(
          _t, std::move(_args)..., std::move(std::get<_j>(_t)));
    }
  }
}

}  // namespace internal
}  // namespace rfl

#endif
