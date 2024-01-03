#ifndef RFL_ARRAY_HPP_
#define RFL_ARRAY_HPP_

#include <type_traits>

#include "internal/to_std_array.hpp"

namespace rfl {

template <class T>
  requires std::is_array_v<T>
struct Array : internal::to_std_array_t<T> {
  using Type = T;
  using Base = internal::to_std_array_t<T>;
  using Base::Base;

  Array() = default;
  Array(const Base &t) : Base(t) {}
  Array(Base &&t) : Base(std::move(t)) {}
  Array(const T &t) : Base(reinterpret_cast<const Base &>(t)) {}
  Array(T &&t) : Base(reinterpret_cast<Base &&>(t)) {}
};

}  // namespace rfl

#endif
