#pragma once

#include "common.hpp"
#include "details/optional.hpp"
#include "enchantum.hpp"
#include <cstddef>

namespace enchantum {
namespace details {
  template<std::ptrdiff_t N>
  struct next_value_functor {
    template<Enum E>
    [[nodiscard]] constexpr optional<E> operator()(const E value, const std::ptrdiff_t n = 1) const noexcept
    {
      if (!enchantum::contains(value))
        return optional<E>{};

      const auto index = static_cast<std::ptrdiff_t>(*enchantum::enum_to_index(value)) + (n * N);
      if (index >= 0 && index < static_cast<std::ptrdiff_t>(count<E>))
        return optional<E>{values<E>[static_cast<std::size_t>(index)]};
      return optional<E>{};
    }
  };

  template<std::ptrdiff_t N>
  struct next_value_circular_functor {
    template<Enum E>
    [[nodiscard]] constexpr E operator()(const E value, const std::ptrdiff_t n = 1) const noexcept
    {
      ENCHANTUM_ASSERT(enchantum::contains(value), "next/prev_value_circular requires 'value' to be a valid enum member", value);
      const auto     i     = static_cast<std::ptrdiff_t>(*enchantum::enum_to_index(value));
      constexpr auto count = static_cast<std::ptrdiff_t>(enchantum::count<E>);
      return values<E>[static_cast<std::size_t>(((i + (n * N)) % count + count) % count)]; // handles wrap around and negative n
    }
  };
} // namespace details


inline constexpr details::next_value_functor<1>           next_value{};
inline constexpr details::next_value_functor<-1>          prev_value{};
inline constexpr details::next_value_circular_functor<1>  next_value_circular{};
inline constexpr details::next_value_circular_functor<-1> prev_value_circular{};

} // namespace enchantum
