#ifndef RFL_INTERNAL_NOFIELDNAMES_HPP_
#define RFL_INTERNAL_NOFIELDNAMES_HPP_

#include <type_traits>

#include "../NoFieldNames.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class no_field_names;

template <class T>
class no_field_names : public std::false_type {};

template <>
class no_field_names<NoFieldNames> : public std::true_type {};

template <class Head, class... Tail>
struct no_field_names<Processors<Head, Tail...>> {
  static constexpr bool value =
      (no_field_names<Head>::value || ... || no_field_names<Tail>::value);
};

template <class T>
constexpr bool no_field_names_v =
    no_field_names<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
