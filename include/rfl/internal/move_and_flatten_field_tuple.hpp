#ifndef RFL_INTERNAL_MOVE_AND_FLATTEN_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_MOVE_AND_FLATTEN_FIELD_TUPLE_HPP_

#include <tuple>

#include "../Tuple.hpp"
#include "../tuple_cat.hpp"
#include "is_flatten_field.hpp"
#include "move_to_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class FieldTuple>
auto move_and_flatten_field_tuple(FieldTuple&& _t) {
  const auto get_one = [&]<int _i>(std::integral_constant<int, _i>) {
    using T = tuple_element_t<_i, std::remove_cvref_t<FieldTuple>>;
    if constexpr (is_flatten_field_v<T>) {
      return move_and_flatten_field_tuple(
          move_to_field_tuple(std::move(rfl::get<_i>(_t).value_)));
    } else {
      return rfl::make_tuple(std::move(rfl::get<_i>(_t)));
    }
  };

  constexpr auto size = rfl::tuple_size_v<std::remove_cvref_t<FieldTuple>>;

  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return rfl::tuple_cat(get_one(std::integral_constant<int, _is>{})...);
  }
  (std::make_integer_sequence<int, size>());
}

}  // namespace internal
}  // namespace rfl

#endif
