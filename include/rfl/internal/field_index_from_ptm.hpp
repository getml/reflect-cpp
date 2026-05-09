#ifndef RFL_INTERNAL_FIELD_INDEX_FROM_PTM_HPP_
#define RFL_INTERNAL_FIELD_INDEX_FROM_PTM_HPP_

#include <cstddef>
#include <utility>

#include "get_field_names.hpp"
#include "get_ith_field_from_fake_object.hpp"
#include "num_fields.hpp"

namespace rfl::internal {

/// Finds the index of the data member designated by `FieldPtr` in T by
/// comparing the address of (obj.*FieldPtr) against the addresses of T's
/// structured-binding fields, where obj is a locally default-constructed T.
/// All comparisons are between subobject pointers of the same local object,
/// which is well-defined in a constant expression. T must be
/// default-constructible in a constant expression.
/// Returns size_t(-1) if no match is found, in which case callers must
/// produce a static_assert.
template <class T, auto FieldPtr, std::size_t... Is>
consteval std::size_t field_index_from_ptm_impl(std::index_sequence<Is...>) {
  T obj{};
  const void* target = static_cast<const void*>(&(obj.*FieldPtr));
  std::size_t result = static_cast<std::size_t>(-1);
  ((static_cast<const void*>(get_ith_field_ptr<T, static_cast<int>(Is)>(obj)) ==
            target
        ? (result = Is, true)
        : false) ||
   ...);
  return result;
}

template <class T, auto FieldPtr>
inline constexpr std::size_t field_index_v =
    field_index_from_ptm_impl<T, FieldPtr>(
        std::make_index_sequence<num_fields<T>>{});

/// Returns the i-th field pointer of the fake object of T in the form
/// `get_field_name_str_lit` expects on the current compiler. On clang the
/// pointer is wrapped in `Wrapper` because clang's parser of __PRETTY_FUNCTION__
/// extracts the field name from the wrapper's type, not from the value.
template <class T, std::size_t I>
consteval auto fake_field_ptr_for_name_lookup() {
#if defined(__clang__)
  return wrap(get_ith_field_from_fake_object<T, static_cast<int>(I)>());
#else
  return get_ith_field_from_fake_object<T, static_cast<int>(I)>();
#endif
}

}  // namespace rfl::internal

#endif
