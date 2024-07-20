#ifndef RFL_INTERNAL_TUPLE_ACCUMULATE_SIZES_HPP_
#define RFL_INTERNAL_TUPLE_ACCUMULATE_SIZES_HPP_

#include <array>
#include <utility>

namespace rfl::internal::tuple {

template <class T>
struct SizeWrapper {};

template <unsigned long _last, unsigned long... _is>
struct Sizes {
  static consteval auto to_array() {
    return std::array<unsigned long, sizeof...(_is)>({_is...});
  }
};

template <class T, unsigned long _last, unsigned long... _is>
consteval auto operator+(const Sizes<_last, _is...>& _sizes,
                         const SizeWrapper<T>& _w) {
  constexpr auto last = _last + alignof(T);
  return Sizes<last, _is..., last>{};
}

template <class... Types>
consteval auto accumulate_sizes() {
  return (Sizes<0, 0>{} + ... + SizeWrapper<Types>{}).to_array();
}

}  // namespace rfl::internal::tuple

#endif
