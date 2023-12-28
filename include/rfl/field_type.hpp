#ifndef RFL_FIELD_TYPE_HPP_
#define RFL_FIELD_TYPE_HPP_

#include <tuple>
#include <type_traits>
#include <variant>

#include "internal/StringLiteral.hpp"
#include "internal/field_type.hpp"

namespace rfl {

template <internal::StringLiteral _field_name, class NamedTupleType>
using field_type_t =
    typename internal::FieldType<_field_name, NamedTupleType>::Type;

}  // namespace rfl

#endif
