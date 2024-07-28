#ifndef RFL_INTERNAL_GETTER_HPP_
#define RFL_INTERNAL_GETTER_HPP_

#include <tuple>
#include <variant>

#include "../Tuple.hpp"
#include "StringLiteral.hpp"
#include "find_index.hpp"

namespace rfl::internal {

// ----------------------------------------------------------------------------

template <class NamedTupleType>
struct Getter;

// ----------------------------------------------------------------------------

/// Default case - anything that cannot be explicitly matched.
template <class NamedTupleType>
struct Getter {
 public:
  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline auto& get(NamedTupleType& _tup) {
    return rfl::get<_index>(_tup.values());
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline auto& get(NamedTupleType& _tup) {
    constexpr auto index =
        find_index<_field_name, typename NamedTupleType::Fields>();
    return Getter<NamedTupleType>::template get<index>(_tup);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline auto& get(NamedTupleType& _tup) {
    constexpr auto index =
        find_index<Field::name_, typename NamedTupleType::Fields>();
    static_assert(
        std::is_same<typename tuple_element_t<
                         index, typename NamedTupleType::Fields>::Type,
                     typename Field::Type>(),
        "If two fields have the same name, "
        "their type must be the same as "
        "well.");
    return Getter<NamedTupleType>::template get<index>(_tup);
  }

  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline const auto& get_const(const NamedTupleType& _tup) {
    return rfl::get<_index>(_tup.values());
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline const auto& get_const(const NamedTupleType& _tup) {
    constexpr auto index =
        find_index<_field_name, typename NamedTupleType::Fields>();
    return Getter<NamedTupleType>::template get_const<index>(_tup);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline const auto& get_const(const NamedTupleType& _tup) {
    constexpr auto index =
        find_index<Field::name_, typename NamedTupleType::Fields>();
    static_assert(
        std::is_same<typename tuple_element_t<
                         index, typename NamedTupleType::Fields>::Type,
                     typename Field::Type>(),
        "If two fields have the same name, "
        "their type must be the same as "
        "well.");
    return Getter<NamedTupleType>::template get_const<index>(_tup);
  }
};

// ----------------------------------------------------------------------------

}  // namespace rfl::internal

#endif
