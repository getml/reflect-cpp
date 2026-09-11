#ifndef RFL_INTERNAL_REMOVE_CVREF_FIELDS_T_HPP_
#define RFL_INTERNAL_REMOVE_CVREF_FIELDS_T_HPP_

#include <type_traits>

#include "../NamedTuple.hpp"

namespace rfl::internal {

template <class T>
struct remove_cvref_fields;

template <class... Fields>
struct remove_cvref_fields<NamedTuple<Fields...>> {
  using Type = NamedTuple<
      Field<Fields::name_, std::remove_cvref_t<typename Fields::Type>>...>;
};

template <class T>
using remove_cvref_fields_t = typename remove_cvref_fields<T>::Type;

}  // namespace rfl::internal

#endif
