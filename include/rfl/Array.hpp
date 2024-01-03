#ifndef RFL_ARRAY_HPP_
#define RFL_ARRAY_HPP_

#include <type_traits>

#include "internal/to_std_array.hpp"

namespace rfl {

template <class T>
requires std::is_array_v<T>
struct Array {
  using Type = T;
  using StdArrayType = internal::to_std_array_t<T>;

  Array() = default;
  Array(const StdArrayType &_arr) : arr_(_arr) {}
  Array(StdArrayType &&_arr) : arr_(std::move(_arr)) {}
  Array(const T &_t) : arr_(reinterpret_cast<const StdArrayType &>(_t)) {}
  Array(T &&_t) : arr_(reinterpret_cast<StdArrayType &&>(_t)) {}

  ~Array() = default;

  StdArrayType arr_;
};

}  // namespace rfl

#endif
