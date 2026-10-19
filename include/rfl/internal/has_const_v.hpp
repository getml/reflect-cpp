#ifndef RFL_INTERNAL_HASCONST_V_HPP_
#define RFL_INTERNAL_HASCONST_V_HPP_

#include <tuple>
#include <type_traits>

#include "../NamedTuple.hpp"
#include "../Tuple.hpp"

namespace rfl::internal {

template <class T>
struct has_const;

template <class... Fields>
struct has_const<NamedTuple<Fields...>> {
  constexpr static bool value =
      (std::is_const_v<std::remove_pointer_t<typename Fields::Type>> || ...);
};

template <class... Fields>
struct has_const<Tuple<Fields...>> {
  constexpr static bool value =
      (std::is_const_v<std::remove_pointer_t<Fields>> || ...);
};

template <class... Fields>
struct has_const<std::tuple<Fields...>> {
  constexpr static bool value =
      (std::is_const_v<std::remove_pointer_t<Fields>> || ...);
};

template <class T>
inline constexpr bool has_const_v = has_const<T>::value;

}  // namespace rfl::internal

#endif
