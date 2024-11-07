#ifndef RFL_INTERNAL_PROCESSORS_HPP_
#define RFL_INTERNAL_PROCESSORS_HPP_

#include <type_traits>

#include "internal/is_add_tags_to_variants_v.hpp"
#include "internal/is_allow_raw_ptrs_v.hpp"
#include "internal/is_default_if_missing_v.hpp"
#include "internal/is_no_extra_fields_v.hpp"
#include "internal/is_no_field_names_v.hpp"
#include "internal/is_no_optionals_v.hpp"
#include "internal/is_underlying_enums_v.hpp"

namespace rfl {

template <class... Ps>
struct Processors;

template <>
struct Processors<> {
  static constexpr bool add_tags_to_variants_ = false;
  static constexpr bool allow_raw_ptrs_ = false;
  static constexpr bool all_required_ = false;
  static constexpr bool default_if_missing_ = false;
  static constexpr bool no_extra_fields_ = false;
  static constexpr bool no_field_names_ = false;
  static constexpr bool underlying_enums_ = false;

  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return _named_tuple;
  }
};

template <class Head, class... Tail>
struct Processors<Head, Tail...> {
  static constexpr bool add_tags_to_variants_ =
      std::disjunction_v<internal::is_add_tags_to_variants<Head>,
                         internal::is_add_tags_to_variants<Tail>...>;

  static constexpr bool allow_raw_ptrs_ =
      std::disjunction_v<internal::is_allow_raw_ptrs<Head>,
                         internal::is_allow_raw_ptrs<Tail>...>;

  static constexpr bool all_required_ =
      std::disjunction_v<internal::is_no_optionals<Head>,
                         internal::is_no_optionals<Tail>...>;

  static constexpr bool default_if_missing_ =
      std::disjunction_v<internal::is_default_if_missing<Head>,
                         internal::is_default_if_missing<Tail>...>;

  static constexpr bool no_extra_fields_ =
      std::disjunction_v<internal::is_no_extra_fields<Head>,
                         internal::is_no_extra_fields<Tail>...>;

  static constexpr bool no_field_names_ =
      std::disjunction_v<internal::is_no_field_names<Head>,
                         internal::is_no_field_names<Tail>...>;

  static constexpr bool underlying_enums_ =
      std::disjunction_v<internal::is_underlying_enums<Head>,
                         internal::is_underlying_enums<Tail>...>;

  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return Processors<Tail...>::template process<T>(
        Head::template process<T>(std::move(_named_tuple)));
  }
};

}  // namespace rfl

#endif
