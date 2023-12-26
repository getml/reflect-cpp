#ifndef RFL_INTERNAL_COPY_FLATTENED_TUPLE_TO_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_COPY_FLATTENED_TUPLE_TO_NAMED_TUPLE_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "../Field.hpp"
#include "lit_name.hpp"
#include "../make_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class FieldNames, class... Fields>
auto copy_flattened_tuple_to_named_tuple(const auto& _flattened_tuple,
                                         Fields&&... _fields) {
  constexpr auto size =
      std::tuple_size_v<std::remove_cvref_t<decltype(_flattened_tuple)>>;
  constexpr auto i = sizeof...(_fields);
  if constexpr (i == size) {
    return make_named_tuple(std::move(_fields)...);
  } else {
    const auto name_literal = FieldNames::template name_of<i>();
    auto new_field = rfl::make_field<
        lit_name_v<std::remove_cvref_t<decltype(name_literal)>>>(
        std::get<i>(_flattened_tuple));
    return copy_flattened_tuple_to_named_tuple<FieldNames>(
        _flattened_tuple, std::move(_fields)..., std::move(new_field));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
