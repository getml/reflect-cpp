#ifndef RFL_INTERNAL_TUPLE_ACCUMULATE_SIZES_HPP_
#define RFL_INTERNAL_TUPLE_ACCUMULATE_SIZES_HPP_

#include <bits/utility.h>

#include <array>
#include <utility>

namespace rfl::internal::tuple {

template <class T>
struct SizeWrapper {};

template <auto _array, unsigned long _val, int... _is>
consteval append(std::integer_sequence<int, _is...>) {
  return std::array<unsigned long, sizeof...(_is) + 1>(
      {std::get<_is>(_array)..., _val});
}

template <class T, int _length>
consteval auto operator+(const std::array<unsigned long, _length>& _arr,
                         const SizeWrapper<T>& _w) {
  return append<_arr, std::get<_length - 1>(_arr) + sizeof(T)>(
      std::make_integer_sequence<int, _length>());
}

template <class... Types>
consteval auto accumulate_sizes() {
  return (std::array<unsigned long, 1>({0}) + ... + SizeWrapper<Types>{});
}

}  // namespace rfl::internal::tuple

#endif
