#ifndef RFL_INTERNAL_NT_TO_PTR_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_NT_TO_PTR_NAMED_TUPLE_HPP_

#include <tuple>

#include "../Field.hpp"
#include "../Tuple.hpp"
#include "../make_named_tuple.hpp"

namespace rfl {
namespace internal {

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a named tuple.
template <class NamedTupleType>
auto nt_to_ptr_named_tuple(NamedTupleType& _nt) {
  using Fields = typename NamedTupleType::Fields;

  const auto get_one = [&]<int _i>(std::integral_constant<int, _i>) {
    using FieldType = tuple_element_t<_i, Fields>;
    using T = std::remove_cvref_t<typename FieldType::Type>;
    return Field<FieldType::name_, T*>(&rfl::get<_i>(_nt.values()));
  };

  constexpr auto num_fields = rfl::tuple_size_v<Fields>;

  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return make_named_tuple(get_one(std::integral_constant<int, _is>{})...);
  }
  (std::make_integer_sequence<int, num_fields>());
}

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a named tuple.
template <class NamedTupleType>
auto nt_to_ptr_named_tuple(const NamedTupleType& _nt) {
  using Fields = typename NamedTupleType::Fields;

  const auto get_one = [&]<int _i>(std::integral_constant<int, _i>) {
    using FieldType = tuple_element_t<_i, Fields>;
    using T = std::remove_cvref_t<typename FieldType::Type>;
    return Field<FieldType::name_, const T*>(&rfl::get<_i>(_nt.values()));
  };

  constexpr auto num_fields = rfl::tuple_size_v<Fields>;

  return [&]<int... _is>(std::integer_sequence<int, _is...>) {
    return make_named_tuple(get_one(std::integral_constant<int, _is>{})...);
  }
  (std::make_integer_sequence<int, num_fields>());
}

}  // namespace internal
}  // namespace rfl

#endif
