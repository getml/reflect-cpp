#ifndef RFL_MAKE_FIELD_HPP_
#define RFL_MAKE_FIELD_HPP_

#include <type_traits>
#include <utility>

#include "Field.hpp"
#include "internal/Array.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/get_member_ptr_name.hpp"

namespace rfl {

template <internal::StringLiteral _name, class T>
inline auto make_field(T&& _value) {
  using T0 = std::remove_cvref_t<T>;
  if constexpr (std::is_array_v<T0>) {
    return Field<_name, T0>(internal::Array<T0>(std::forward<T>(_value)));
  } else {
    return Field<_name, T0>(std::forward<T>(_value));
  }
}

template <auto ptr, class T>
  requires(std::is_member_pointer_v<decltype(ptr)>)
inline auto make_field(T&& _value) {
  return make_field<internal::get_member_ptr_name<ptr>()>(
      std::forward<T>(_value));
}

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
