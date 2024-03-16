#ifndef RFL_INTERNAL_ARRAY_HPP_
#define RFL_INTERNAL_ARRAY_HPP_

#include <array>
#include <cstddef>
#include <type_traits>

#include "to_std_array.hpp"

namespace rfl {
namespace internal {

template <class T>
requires std::is_array_v<T>
struct Array {
  using Type = T;
  using StdArrayType = to_std_array_t<T>;

  Array() = default;
  Array(const StdArrayType &_arr) : arr_(_arr) {}
  Array(StdArrayType &&_arr) : arr_(std::move(_arr)) {}
  Array(const T &_arr) : arr_(to_std_array(_arr)) {}
  Array(T &&_arr) : arr_(to_std_array(_arr)) {}

  ~Array() = default;

  StdArrayType arr_;
};

}  // namespace internal
}  // namespace rfl

#endif
