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
    return std::array<unsigned long, sizeof...(_is) + 1>({_is..., _last});
  }
};

template <class T, unsigned long _last, unsigned long... _is>
consteval auto operator+(const Sizes<_last, _is...>& _sizes,
                         const SizeWrapper<T>& _w) {
  if constexpr (_last % alignof(T) == 0) {
    constexpr auto last_new = _last + sizeof(T);
    return Sizes<last_new, _is..., _last>{};
  } else {
    constexpr auto last_corrected = _last + alignof(T) - (_last % alignof(T));
    constexpr auto last_new = last_corrected + sizeof(T);
    return Sizes<last_new, _is..., last_corrected>{};
  }
}

template <class... Types>
consteval auto accumulate_sizes() {
  return (Sizes<0>{} + ... + SizeWrapper<Types>{}).to_array();
}

}  // namespace rfl::internal::tuple

#endif
