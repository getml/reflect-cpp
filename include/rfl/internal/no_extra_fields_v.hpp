#ifndef RFL_INTERNAL_NOEXTRAFIELDS_HPP_
#define RFL_INTERNAL_NOEXTRAFIELDS_HPP_

#include <type_traits>

#include "../NoExtraFields.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class no_extra_fields;

template <class T>
class no_extra_fields : public std::false_type {};

template <>
class no_extra_fields<NoExtraFields> : public std::true_type {};

template <class Head, class... Tail>
struct no_extra_fields<Processors<Head, Tail...>> {
  static constexpr bool value =
      (no_extra_fields<Head>::value || ... || no_extra_fields<Tail>::value);
};

template <class T>
constexpr bool no_extra_fields_v =
    no_extra_fields<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
