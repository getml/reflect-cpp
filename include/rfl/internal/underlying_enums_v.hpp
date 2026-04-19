#ifndef RFL_INTERNAL_UNDERLYINGENUMS_HPP_
#define RFL_INTERNAL_UNDERLYINGENUMS_HPP_

#include <type_traits>

#include "../Processors.hpp"
#include "../UnderlyingEnums.hpp"

namespace rfl ::internal {

template <class T>
class is_underlying_enums;

template <class T>
class is_underlying_enums : public std::false_type {};

template <>
class is_underlying_enums<UnderlyingEnums> : public std::true_type {};

template <class Head, class... Tail>
struct is_underlying_enums<Processors<Head, Tail...>> {
  static constexpr bool value = (is_underlying_enums<Head>::value || ... ||
                                 is_underlying_enums<Tail>::value);
};

template <class T>
constexpr bool is_underlying_enums_v =
    is_underlying_enums<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
