#ifndef RFL_INTERNAL_NO_DUPLICATE_FIELD_NAMES_HPP_
#define RFL_INTERNAL_NO_DUPLICATE_FIELD_NAMES_HPP_

#include "../Tuple.hpp"

namespace rfl {
namespace internal {

namespace no_duplicate_field_names_helpers {

template <class Fields, int _i, int _j>
constexpr inline void compare_two_fields() {
  if constexpr (_j < _i) {
    using FieldType1 = std::remove_cvref_t<tuple_element_t<_i, Fields>>;
    using FieldType2 = std::remove_cvref_t<tuple_element_t<_j, Fields>>;

    constexpr auto field_name_i = FieldType1::name_;
    constexpr auto field_name_j = FieldType2::name_;

    constexpr bool no_duplicate = (field_name_i != field_name_j);

    static_assert(no_duplicate,
                  "Duplicate field names are not allowed in either named "
                  "tuples or Literals.");
  }
}

template <class Fields, int _i, int... _js>
constexpr inline void iterate_over_j(std::integer_sequence<int, _js...>) {
  (compare_two_fields<Fields, _i, _js>(), ...);
}

template <class Fields, int... _is>
constexpr inline void iterate_over_i(std::integer_sequence<int, _is...>) {
  (iterate_over_j<Fields, _is>(std::make_integer_sequence<int, _is>()), ...);
}
}  // namespace no_duplicate_field_names_helpers

template <class Fields>
constexpr inline bool no_duplicate_field_names() {
  constexpr auto num_fields = rfl::tuple_size_v<Fields>;

  if constexpr (num_fields <= 1) {
    return true;
  } else {
    no_duplicate_field_names_helpers::iterate_over_i<Fields>(
        std::make_integer_sequence<int, num_fields>());
    return true;
  }
}

}  // namespace internal
}  // namespace rfl

#endif
