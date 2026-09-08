#ifndef RFL_INTERNAL_UNDERLYINGENUMS_HPP_
#define RFL_INTERNAL_UNDERLYINGENUMS_HPP_

#include <type_traits>

#include "../Processors.hpp"
#include "../UnderlyingEnums.hpp"

namespace rfl::internal {

template <class T>
class underlying_enums;

template <class T>
class underlying_enums : public std::false_type {};

template <>
class underlying_enums<UnderlyingEnums> : public std::true_type {};

template <class Head, class... Tail>
struct underlying_enums<Processors<Head, Tail...>> {
  static constexpr bool value =
      (underlying_enums<Head>::value || ... || underlying_enums<Tail>::value);
};

template <class T>
constexpr bool underlying_enums_v =
    underlying_enums<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
