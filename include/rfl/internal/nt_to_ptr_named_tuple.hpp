#ifndef RFL_INTERNAL_NT_TO_PTR_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_NT_TO_PTR_NAMED_TUPLE_HPP_

#include <tuple>

#include "../Field.hpp"
#include "../make_named_tuple.hpp"

namespace rfl {
namespace internal {

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a named tuple.
template <class NamedTupleType, class... AlreadyExtracted>
auto nt_to_ptr_named_tuple(NamedTupleType& _nt, AlreadyExtracted... _a) {
  using Fields = typename NamedTupleType::Fields;

  constexpr auto i = sizeof...(AlreadyExtracted);
  constexpr auto num_fields = std::tuple_size_v<Fields>;

  if constexpr (i == num_fields) {
    return make_named_tuple(_a...);
  } else {
    using FieldType = typename std::tuple_element<i, Fields>::type;
    using T = std::remove_cvref_t<typename FieldType::Type>;
    return nt_to_ptr_named_tuple(
        _nt, _a..., Field<FieldType::name_, T*>(&std::get<i>(_nt.values())));
  }
}

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a named tuple.
template <class NamedTupleType, class... AlreadyExtracted>
auto nt_to_ptr_named_tuple(const NamedTupleType& _nt, AlreadyExtracted... _a) {
  using Fields = typename NamedTupleType::Fields;

  constexpr auto i = sizeof...(AlreadyExtracted);
  constexpr auto num_fields = std::tuple_size_v<Fields>;

  if constexpr (i == num_fields) {
    return make_named_tuple(_a...);
  } else {
    using FieldType = typename std::tuple_element<i, Fields>::type;
    using T = std::remove_cvref_t<typename FieldType::Type>;
    return nt_to_ptr_named_tuple(
        _nt, _a...,
        Field<FieldType::name_, const T*>(&std::get<i>(_nt.values())));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
