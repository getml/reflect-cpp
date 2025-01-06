#ifndef RFL_INTERNAL_REMOVE_PTRS_NT_HPP_
#define RFL_INTERNAL_REMOVE_PTRS_NT_HPP_

#include <functional>
#include <type_traits>

#include "../Field.hpp"
#include "StringLiteral.hpp"
#include "wrap_in_rfl_array_t.hpp"

namespace rfl::internal {

template <class T>
struct remove_ptr;

template <internal::StringLiteral _name, class T>
struct remove_ptr<Field<_name, T>> {
  using FieldType =
      Field<_name, internal::wrap_in_rfl_array_t<
                       std::remove_cvref_t<std::remove_pointer_t<T>>>>;
};

template <class T>
struct remove_ptrs_nt;

template <class... FieldTypes>
struct remove_ptrs_nt<NamedTuple<FieldTypes...>> {
  using NamedTupleType =
      NamedTuple<typename remove_ptr<FieldTypes>::FieldType...>;
};

}  // namespace rfl::internal

#endif
