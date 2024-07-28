#ifndef RFL_INTERNAL_COPY_FLATTENED_TUPLE_TO_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_COPY_FLATTENED_TUPLE_TO_NAMED_TUPLE_HPP_

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../Field.hpp"
#include "../make_named_tuple.hpp"
#include "lit_name.hpp"

namespace rfl {
namespace internal {

template <class FieldNames, int _i>
auto copy_one_element(const auto& _flattened_tuple) {
  const auto name_literal = FieldNames::template name_of<_i>();
  return rfl::make_field<
      lit_name_v<std::remove_cvref_t<decltype(name_literal)>>>(
      std::get<_i>(_flattened_tuple));
}

template <class FieldNames, class... Fields>
auto copy_flattened_tuple_to_named_tuple(const auto& _flattened_tuple) {
  constexpr auto size =
      rfl::tuple_size_v<std::remove_cvref_t<decltype(_flattened_tuple)>>;
  return [&]<int... _i>(std::integer_sequence<int, _i...>) {
    return make_named_tuple(
        copy_one_element<FieldNames, _i>(_flattened_tuple)...);
  }
  (std::make_integer_sequence<int, size>());
}

}  // namespace internal
}  // namespace rfl

#endif
