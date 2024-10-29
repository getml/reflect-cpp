#ifndef RFL_INTERNAL_TO_FLATTENED_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TO_FLATTENED_PTR_TUPLE_HPP_

#include <tuple>

#include "../Tuple.hpp"
#include "../tuple_cat.hpp"
#include "has_flatten_fields.hpp"
#include "is_flatten_field.hpp"
#include "to_ptr_tuple.hpp"

namespace rfl {
namespace internal {

template <class PtrTuple>
auto flatten_ptr_tuple(PtrTuple&& _t) {
  if constexpr (!has_flatten_fields<PtrTuple>()) {
    return std::forward<PtrTuple>(_t);
  } else {
    const auto get_one = [&]<int _i>(std::integral_constant<int, _i>) {
      using T = tuple_element_t<_i, std::remove_cvref_t<PtrTuple>>;
      if constexpr (is_flatten_field_v<T>) {
        return flatten_ptr_tuple(to_ptr_tuple(rfl::get<_i>(_t)->get()));
      } else {
        return rfl::make_tuple(rfl::get<_i>(_t));
      }
    };

    constexpr auto size = rfl::tuple_size_v<std::remove_cvref_t<PtrTuple>>;

    return [&]<int... _is>(std::integer_sequence<int, _is...>) {
      return rfl::tuple_cat(get_one(std::integral_constant<int, _is>{})...);
    }
    (std::make_integer_sequence<int, size>());
  }
}

template <class T>
auto to_flattened_ptr_tuple(T&& _t) {
  return flatten_ptr_tuple(to_ptr_tuple(_t));
}

}  // namespace internal
}  // namespace rfl

#endif
