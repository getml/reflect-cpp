#ifndef RFL_INTERNAL_REMOVEFIELDS_HPP_
#define RFL_INTERNAL_REMOVEFIELDS_HPP_

#include <algorithm>
#include <tuple>
#include <type_traits>

#include "../NamedTuple.hpp"
#include "../Tuple.hpp"
#include "../define_named_tuple.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

/// Recursively builds a new NamedTuple type from the FieldTypes, leaving out
/// the field signified by _name.
template <class _OldNamedTupleType, StringLiteral _name,
          class _NewNamedTupleType, int _i>
struct remove_single_field;

/// Special case - _i == 0
template <class _OldNamedTupleType, StringLiteral _name,
          class _NewNamedTupleType>
struct remove_single_field<_OldNamedTupleType, _name, _NewNamedTupleType, 0> {
  using type = _NewNamedTupleType;
};

/// General case.
template <class _OldNamedTupleType, StringLiteral _name,
          class _NewNamedTupleType, int _i>
struct remove_single_field {
  using OldNamedTupleType = std::remove_cvref_t<_OldNamedTupleType>;

  constexpr static int num_fields =
      rfl::tuple_size_v<typename OldNamedTupleType::Fields>;

  using FieldType = std::remove_cvref_t<
      tuple_element_t<num_fields - _i, typename OldNamedTupleType::Fields>>;

  using NewNamedTupleType =
      std::conditional_t<_name == FieldType::name_, _NewNamedTupleType,
                         define_named_tuple_t<_NewNamedTupleType, FieldType>>;

  using type = typename remove_single_field<OldNamedTupleType, _name,
                                            NewNamedTupleType, _i - 1>::type;
};

/// Recursively removes all of the fields signified by _head and _tail from the
/// NamedTupleType.
template <class _NamedTupleType, StringLiteral _head, StringLiteral... _tail>
struct remove_fields;

/// Special case - only head is left.
template <class _NamedTupleType, StringLiteral _head>
struct remove_fields<_NamedTupleType, _head> {
  using NamedTupleType = std::remove_cvref_t<_NamedTupleType>;

  constexpr static int num_fields =
      rfl::tuple_size_v<typename NamedTupleType::Fields>;

  using type = typename remove_single_field<NamedTupleType, _head, NamedTuple<>,
                                            num_fields>::type;
};

/// General case.
template <class _NamedTupleType, StringLiteral _head, StringLiteral... _tail>
struct remove_fields {
  using NamedTupleType = std::remove_cvref_t<_NamedTupleType>;

  constexpr static int num_fields =
      rfl::tuple_size_v<typename NamedTupleType::Fields>;

  using NewNamedTupleType =
      typename remove_single_field<NamedTupleType, _head, NamedTuple<>,
                                   num_fields>::type;

  using type = typename remove_fields<NewNamedTupleType, _tail...>::type;
};

}  // namespace internal
}  // namespace rfl

#endif  // RFL_REMOVEFIELDS_HPP_
