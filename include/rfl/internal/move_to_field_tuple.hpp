#ifndef RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_MOVE_TO_FIELD_TUPLE_HPP_

#include <tuple>
#include <type_traits>

#include "rfl/field_names_t.hpp"
#include "rfl/internal/flattened_ptr_tuple_t.hpp"
#include "rfl/internal/has_fields.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/lit_name.hpp"
#include "rfl/internal/move_to_field_tuple_impl.hpp"

namespace rfl {
namespace internal {

template <class FieldNames, int j = 0, class... Fields>
auto wrap_in_fields(auto&& _tuple, Fields&&... _fields) {
  constexpr auto size = std::tuple_size_v<std::decay_t<decltype(_tuple)>>;
  constexpr auto i = sizeof...(_fields);
  if constexpr (i == size) {
    return std::make_tuple(std::move(_fields)...);

  } else {
    auto value = std::move(std::get<i>(_tuple));
    using Type = std::decay_t<decltype(value)>;
    if constexpr (is_flatten_field_v<Type>) {
      // The problem here is that the FieldNames are already flattened, but this
      // is not, so we need to determine how many field names to skip.
      constexpr auto n_skip = std::tuple_size_v<
          std::decay_t<flattened_ptr_tuple_t<typename Type::Type>>>;
      return wrap_in_fields<FieldNames, j + n_skip>(
          std::move(_tuple), std::move(_fields)..., std::move(value));
    } else {
      const auto name_literal = FieldNames::template name_of<j>();
      auto new_field =
          rfl::make_field<lit_name_v<std::decay_t<decltype(name_literal)>>>(
              std::move(value));
      return wrap_in_fields<FieldNames, j + 1>(
          std::move(_tuple), std::move(_fields)..., std::move(new_field));
    }
  }
}

template <class OriginalStruct>
auto move_to_field_tuple(OriginalStruct&& _t) {
  using T = std::decay_t<OriginalStruct>;
  if constexpr (internal::is_named_tuple_v<T>) {
    return _t.fields();
  } else if constexpr (has_fields<T>()) {
    return move_to_field_tuple_impl(std::move(_t));
  } else {
    using FieldNames = field_names_t<T>;
    auto tup = move_to_field_tuple_impl(std::move(_t));
    return wrap_in_fields<FieldNames>(std::move(tup));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
