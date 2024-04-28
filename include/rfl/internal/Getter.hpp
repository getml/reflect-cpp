#ifndef RFL_INTERNAL_GETTER_HPP_
#define RFL_INTERNAL_GETTER_HPP_

#include <tuple>
#include <variant>

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
    return std::get<_index>(_tup.values());
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
        std::is_same<typename std::tuple_element<
                         index, typename NamedTupleType::Fields>::type::Type,
                     typename Field::Type>(),
        "If two fields have the same name, "
        "their type must be the same as "
        "well.");
    return Getter<NamedTupleType>::template get<index>(_tup);
  }

  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline const auto& get_const(const NamedTupleType& _tup) {
    return std::get<_index>(_tup.values());
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
        std::is_same<typename std::tuple_element<
                         index, typename NamedTupleType::Fields>::type::Type,
                     typename Field::Type>(),
        "If two fields have the same name, "
        "their type must be the same as "
        "well.");
    return Getter<NamedTupleType>::template get_const<index>(_tup);
  }
};

// ----------------------------------------------------------------------------

/// For handling std::variant.
template <class... NamedTupleTypes>
struct Getter<std::variant<NamedTupleTypes...>> {
 public:
  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline auto& get(std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](auto& _t) -> auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_t)>;
      return Getter<NamedTupleType>::template get<_index>(_t);
    };
    return std::visit(apply, _tup);
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline auto& get(std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](auto& _t) -> auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_t)>;
      return Getter<NamedTupleType>::template get<_field_name>(_t);
    };
    return std::visit(apply, _tup);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline auto& get(std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](auto& _t) -> auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_t)>;
      return Getter<NamedTupleType>::template get<Field>(_t);
    };
    return std::visit(apply, _tup);
  }

  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline const auto& get_const(
      const std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](const auto& _tup) -> const auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_tup)>;
      return Getter<NamedTupleType>::template get_const<_index>(_tup);
    };
    return std::visit(apply, _tup);
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline const auto& get_const(
      const std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](const auto& _t) -> const auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_t)>;
      return Getter<NamedTupleType>::template get_const<_field_name>(_t);
    };
    return std::visit(apply, _tup);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline const auto& get_const(
      const std::variant<NamedTupleTypes...>& _tup) {
    const auto apply = [](const auto& _t) -> const auto& {
      using NamedTupleType = std::remove_cvref_t<decltype(_t)>;
      return Getter<NamedTupleType>::template get_const<Field>(_t);
    };
    return std::visit(apply, _tup);
  }
};

// ----------------------------------------------------------------------------

}  // namespace rfl::internal

#endif
