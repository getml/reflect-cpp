#ifndef RFL_INTERNAL_FIELD_INDEX_BY_NAME_HPP_
#define RFL_INTERNAL_FIELD_INDEX_BY_NAME_HPP_

#include <cstddef>
#include <string_view>
#include <utility>

#include "StringLiteral.hpp"
#include "field_index_from_ptm.hpp"
#include "get_field_names.hpp"
#include "get_ith_field_from_fake_object.hpp"
#include "num_fields.hpp"

namespace rfl::internal {

/// Returns the name of the i-th field of T as a string_view, using the same
/// fake-object name-lookup path that to_named_tuple uses.
template <class T, std::size_t I>
consteval std::string_view field_name_at() {
  return get_field_name_str_view<T,
                                 fake_field_ptr_for_name_lookup<T, I>()>();
}

template <class T, StringLiteral Name, std::size_t... Is>
consteval std::size_t field_index_by_name_impl(std::index_sequence<Is...>) {
  std::size_t result = static_cast<std::size_t>(-1);
  ((field_name_at<T, Is>() == Name.string_view()
        ? (result = Is, true)
        : false) ||
   ...);
  return result;
}

/// Finds the index of the field with the given Name in T. Returns size_t(-1)
/// if no field with that name exists; callers must produce a static_assert.
template <class T, StringLiteral Name>
inline constexpr std::size_t field_index_by_name_v =
    field_index_by_name_impl<T, Name>(
        std::make_index_sequence<num_fields<T>>{});

/// Helper: extracts the cv-stripped value type of the i-th field of T,
/// suitable as a function parameter or local variable type. Use this when
/// you need to declare an object of the field's type; do not use it to
/// inspect const-ness of the original field declaration (cv is removed).
template <class T, std::size_t I>
struct field_value_type_at {
  using type = std::remove_cv_t<std::remove_pointer_t<
      decltype(get_ith_field_from_fake_object<T, static_cast<int>(I)>())>>;
};

/// Sentinel value type used when field_index_by_name_v fails to find a
/// matching field. The caller's static_assert then fires before any
/// real call site can use this type.
struct unresolved_field_type {};

/// When the index is the sentinel size_t(-1), return a placeholder type so
/// that a failing field_index_by_name_v lookup does not trip an out-of-range
/// instantiation before the caller's static_assert can fire.
template <class T>
struct field_value_type_at<T, static_cast<std::size_t>(-1)> {
  using type = unresolved_field_type;
};

template <class T, std::size_t I>
using field_value_type_at_t = typename field_value_type_at<T, I>::type;

}  // namespace rfl::internal

#endif
