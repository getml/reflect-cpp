#ifndef RFL_INTERNAL_TOPTRFIELD_HPP_
#define RFL_INTERNAL_TOPTRFIELD_HPP_

#include <type_traits>

#include "rfl/Field.hpp"
#include "rfl/Flatten.hpp"
#include "rfl/internal/StringLiteral.hpp"

namespace rfl {
namespace internal {

template <internal::StringLiteral _name, class _Type>
inline auto to_ptr_field(Field<_name, _Type>& _field) {
    using T = std::remove_reference_t<_Type>;
    return Field<_name, T*>(&_field.value_);
}

template <internal::StringLiteral _name, class _Type>
inline auto to_ptr_field(const Field<_name, _Type>& _field) {
    using T = std::remove_cvref_t<_Type>;
    return Field<_name, const T*>(&_field.value_);
}

template <class _Type>
inline auto to_ptr_field(Flatten<_Type>& _field) {
    using T = std::remove_reference_t<_Type>;
    return Flatten<T*>(&_field.value_);
}

template <class _Type>
inline auto to_ptr_field(const Flatten<_Type>& _field) {
    using T = std::remove_cvref_t<_Type>;
    return Flatten<const T*>(&_field.value_);
}

}  // namespace internal
}  // namespace rfl

#endif
