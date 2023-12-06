#ifndef RFL_INTERNAL_GETFIELDNAMEPTRS_HPP_
#define RFL_INTERNAL_GETFIELDNAMEPTRS_HPP_

#include <array>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <type_traits>

#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/is_rename.hpp"
#include "rfl/internal/to_ptr_tuple.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

namespace rfl {
namespace internal {

/// Returns a tuple of pointers required to extract the field names.
template <class T>
constexpr auto get_field_name_ptrs() {
  return to_ptr_tuple(fake_object<T>);
}

}  // namespace internal
}  // namespace rfl

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
