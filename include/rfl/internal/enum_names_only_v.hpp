#ifndef RFL_INTERNAL_ENUMNAMESONLY_HPP_
#define RFL_INTERNAL_ENUMNAMESONLY_HPP_

#include <type_traits>

#include "../EnumNamesOnly.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class enum_names_only;

template <class T>
class enum_names_only : public std::false_type {};

template <>
class enum_names_only<EnumNamesOnly> : public std::true_type {};

template <class Head, class... Tail>
struct enum_names_only<Processors<Head, Tail...>> {
  static constexpr bool value =
      (enum_names_only<Head>::value || ... || enum_names_only<Tail>::value);
};

template <class T>
constexpr bool enum_names_only_v =
    enum_names_only<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
