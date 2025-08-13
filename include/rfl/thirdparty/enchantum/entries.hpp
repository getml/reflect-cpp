#pragma once

#include "details/string_view.hpp"

#if defined(__clang__)
  #include "details/enchantum_clang.hpp"
#elif defined(__GNUC__) || defined(__GNUG__)
  #include "details/enchantum_gcc.hpp"
#elif defined(_MSC_VER)
  #include "details/enchantum_msvc.hpp"
#endif

#include "common.hpp"
#include <type_traits>
#include <utility>

namespace enchantum {

#ifdef __cpp_lib_to_underlying
using ::std::to_underlying;
#else
template<Enum E>
[[nodiscard]] constexpr auto to_underlying(const E e) noexcept
{
  return static_cast<std::underlying_type_t<E>>(e);
}
#endif

template<Enum E, typename Pair = std::pair<E, string_view>, bool ShouldNullTerminate = true>
inline constexpr auto entries = details::reflect<std::remove_cv_t<E>, Pair, ShouldNullTerminate>();

template<Enum E>
inline constexpr auto values = []() {
  constexpr auto&             enums = entries<E>;
  std::array<E, enums.size()> ret;
  for (std::size_t i = 0; i < ret.size(); ++i)
    ret[i] = enums[i].first;
  return ret;
}();

template<Enum E, typename String = string_view, bool NullTerminated = true>
inline constexpr auto names = []() {
  constexpr auto&                  enums = entries<E, std::pair<E, String>, NullTerminated>;
  std::array<String, enums.size()> ret;
  for (std::size_t i = 0; i < ret.size(); ++i)
    ret[i] = enums[i].second;
  return ret;
}();

template<Enum E>
inline constexpr auto min = entries<E>.front().first;

template<Enum E>
inline constexpr auto max = entries<E>.back().first;

template<Enum E>
inline constexpr std::size_t count = entries<E>.size();

} // namespace enchantum