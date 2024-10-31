#ifndef RFL_INTERNAL_TUPLE_CALCULATE_POSITIONS_HPP_
#define RFL_INTERNAL_TUPLE_CALCULATE_POSITIONS_HPP_

#include <array>
#include <utility>

namespace rfl::internal::tuple {

template <class T>
struct PositionWrapper {};

template <unsigned long _last, unsigned long... _is>
struct Positions {
  static consteval auto to_array() {
    return std::array<unsigned long, sizeof...(_is) + 1>({_is..., _last});
  }
};

template <class T, unsigned long _last, unsigned long... _is>
consteval auto operator+(const Positions<_last, _is...>&,
                         const PositionWrapper<T>&) {
  if constexpr (_last % alignof(T) == 0) {
    constexpr auto last_new = _last + sizeof(T);
    return Positions<last_new, _is..., _last>{};
  } else {
    constexpr auto last_corrected = _last + alignof(T) - (_last % alignof(T));
    constexpr auto last_new = last_corrected + sizeof(T);
    return Positions<last_new, _is..., last_corrected>{};
  }
}

template <class... Types>
consteval auto calculate_positions() {
  return (Positions<0>{} + ... + PositionWrapper<Types>{}).to_array();
}

}  // namespace rfl::internal::tuple

#endif
