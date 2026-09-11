#ifndef RFL_INTERNAL_HASCONST_V_HPP_
#define RFL_INTERNAL_HASCONST_V_HPP_

#include <type_traits>

#include "../NamedTuple.hpp"

namespace rfl::internal {

template <class T>
struct has_const;

template <class... Fields>
struct has_const<NamedTuple<Fields...>> {
  constexpr static bool value =
      (std::is_const_v<std::remove_pointer_t<typename Fields::Type>> || ...);
};

template <class T>
inline constexpr bool has_const_v = has_const<T>::value;

}  // namespace rfl::internal

#endif
