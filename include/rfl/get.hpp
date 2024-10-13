#ifndef RFL_GET_HPP_
#define RFL_GET_HPP_

#include "internal/Getter.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Gets a field by index.
template <int _index, class NamedTupleType>
inline auto& get(NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get<_index>(_tup);
}

/// Gets a field by name.
template <internal::StringLiteral _field_name, class NamedTupleType>
inline auto& get(NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get<_field_name>(_tup);
}

/// Gets a field by the field type.
template <class Field, class NamedTupleType>
inline auto& get(NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get<Field>(_tup);
}

/// Gets a field by index.
template <int _index, class NamedTupleType>
inline const auto& get(const NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get_const<_index>(_tup);
}

/// Gets a field by name.
template <internal::StringLiteral _field_name, class NamedTupleType>
inline const auto& get(const NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get_const<_field_name>(
      _tup);
}

/// Gets a field by the field type.
template <class Field, class NamedTupleType>
inline const auto& get(const NamedTupleType& _tup) {
  return internal::Getter<NamedTupleType>::template get_const<Field>(_tup);
}

}  // namespace rfl

#endif
