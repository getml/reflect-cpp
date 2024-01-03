#ifndef RFL_INTERNAL_MOVE_AND_FLATTEN_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_MOVE_AND_FLATTEN_FIELD_TUPLE_HPP_

#include <tuple>

#include "is_flatten_field.hpp"
#include "move_to_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class FieldTuple, class... Args>
auto move_and_flatten_field_tuple(FieldTuple&& _t, Args&&... _args) {
  constexpr auto i = sizeof...(Args);
  if constexpr (i == std::tuple_size_v<std::remove_cvref_t<FieldTuple>>) {
    return std::tuple_cat(std::move(_args)...);
  } else {
    using T = std::tuple_element_t<i, std::remove_cvref_t<FieldTuple>>;
    if constexpr (is_flatten_field<T>::value) {
      return move_and_flatten_field_tuple(
          std::move(_t), std::move(_args)...,
          move_and_flatten_field_tuple(
              move_to_field_tuple(std::move(std::get<i>(_t).value_))));
    } else {
      return move_and_flatten_field_tuple(
          std::move(_t), std::move(_args)...,
          std::make_tuple(std::move(std::get<i>(_t))));
    }
  }
}

}  // namespace internal
}  // namespace rfl

#endif
