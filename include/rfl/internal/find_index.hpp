#ifndef RFL_FIND_INDEX_HPP_
#define RFL_FIND_INDEX_HPP_

#include <type_traits>

#include "StringLiteral.hpp"
#include "nth_tuple_element_t.hpp"

namespace rfl {
namespace internal {

/// Finds the index of the field signified by _field_name
// TODO: Non-recursive implementation
template <StringLiteral _field_name, class Fields, int I = 0>
constexpr static int find_index() {
  using FieldType = std::remove_cvref_t<nth_tuple_element_t<I, Fields>>;

  constexpr bool name_i_matches = (FieldType::name_ == _field_name);

  if constexpr (name_i_matches) {
    return I;
  } else {
    constexpr bool out_of_range = I + 1 == rfl::tuple_size_v<Fields>;

    static_assert(!out_of_range, "Field name not found!");

    if constexpr (out_of_range) {
      // This is to avoid very confusing error messages.
      return I;
    } else {
      return find_index<_field_name, Fields, I + 1>();
    }
  }
}

}  // namespace internal
}  // namespace rfl

#endif
