#ifndef RFL_INTERNAL_FIELD_TYPE_HPP_
#define RFL_INTERNAL_FIELD_TYPE_HPP_

#include <tuple>
#include <type_traits>
#include <variant>

#include "../NamedTuple.hpp"
#include "../TaggedUnion.hpp"
#include "../Tuple.hpp"
#include "../named_tuple_t.hpp"
#include "StringLiteral.hpp"
#include "find_index.hpp"

namespace rfl {
namespace internal {

template <class T, class... Ts>
struct are_same : std::conjunction<std::is_same<T, Ts>...> {};

/// Finds the type of the field signified by _field_name
template <StringLiteral _field_name, class T>
struct FieldType;

/// Default option - for named tuples.
template <StringLiteral _field_name, class T>
struct FieldType {
  using NamedTupleType = named_tuple_t<T>;

  static constexpr int field_ix_ =
      internal::find_index<_field_name, typename NamedTupleType::Fields>();

  using Type = typename tuple_element_t<field_ix_,
                                        typename NamedTupleType::Fields>::Type;
};

/// For variants - in this case the FieldType returned by all options must be
/// the same.
template <StringLiteral _field_name, class FirstAlternativeType,
          class... OtherAlternativeTypes>
struct FieldType<_field_name,
                 std::variant<FirstAlternativeType, OtherAlternativeTypes...>> {
  constexpr static bool all_types_match = std::conjunction_v<std::is_same<
      typename FieldType<_field_name, FirstAlternativeType>::Type,
      typename FieldType<_field_name, OtherAlternativeTypes>::Type>...>;

  static_assert(all_types_match, "All field types must be the same.");

  using Type = typename FieldType<_field_name, FirstAlternativeType>::Type;
};

/// For tagged union - just defers to the variant.
template <StringLiteral _field_name, StringLiteral _discriminator_name,
          class... VarTypes>
struct FieldType<_field_name, TaggedUnion<_discriminator_name, VarTypes...>> {
  using Type =
      typename FieldType<_field_name,
                         typename TaggedUnion<_discriminator_name,
                                              VarTypes...>::VariantType>::Type;
};

}  // namespace internal
}  // namespace rfl

#endif
