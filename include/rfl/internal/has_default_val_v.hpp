#ifndef RFL_HASDEFAULTVALV_HPP_
#define RFL_HASDEFAULTVALV_HPP_

#include <array>
#include <type_traits>

#include "../NamedTuple.hpp"
#include "../named_tuple_t.hpp"
#include "is_default_val_v.hpp"

namespace rfl::internal {

template <class T>
struct HasDefaultVal;

template <class T>
struct HasDefaultVal {
  static constexpr bool value = false;
};

template <class... Fields>
struct HasDefaultVal<NamedTuple<Fields...>> {
  static constexpr bool value =
      (false || ... ||
       is_default_val_v<
           std::remove_cvref_t<std::remove_pointer_t<typename Fields::Type>>>);
};

template <class T>
  requires(std::is_class_v<T> && std::is_aggregate_v<T>)
struct HasDefaultVal<T> : HasDefaultVal<named_tuple_t<T>> {};

template <class T, size_t N>
struct HasDefaultVal<std::array<T, N>> : HasDefaultVal<std::remove_cvref_t<T>> {
};

template <class T, size_t N>
struct HasDefaultVal<T[N]> : HasDefaultVal<std::remove_cvref_t<T>> {};

template <class T>
constexpr bool has_default_val_v = HasDefaultVal<std::remove_cvref_t<T>>::value;

}  // namespace rfl::internal

#endif
