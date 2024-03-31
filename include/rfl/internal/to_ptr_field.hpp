#ifndef RFL_INTERNAL_TOPTRFIELD_HPP_
#define RFL_INTERNAL_TOPTRFIELD_HPP_

#include <type_traits>

#include "../Field.hpp"
#include "../Flatten.hpp"
#include "../always_false.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <internal::StringLiteral _name, class Type>
inline auto to_ptr_field(Field<_name, Type>& _field) {
  using T = std::remove_reference_t<Type>;
  return Field<_name, T*>(&_field.value_);
}

template <internal::StringLiteral _name, class Type>
inline auto to_ptr_field(const Field<_name, Type>& _field) {
  using T = std::remove_cvref_t<Type>;
  return Field<_name, const T*>(&_field.value_);
}

template <class Type>
inline auto to_ptr_field(Flatten<Type>& _field) {
  using T = std::remove_reference_t<Type>;
  return Flatten<T*>(&_field.value_);
}

template <class Type>
inline auto to_ptr_field(const Flatten<Type>& _field) {
  using T = std::remove_cvref_t<Type>;
  return Flatten<const T*>(&_field.value_);
}

}  // namespace internal
}  // namespace rfl

#endif
