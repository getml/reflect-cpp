#pragma once

#include "bitflags.hpp"
#include "details/format_util.hpp"
#include "enchantum.hpp"
#include <fmt/format.h>

template<enchantum::Enum E>
struct fmt::formatter<E> : fmt::formatter<string_view> {
  template<typename FmtContext>
  constexpr auto format(const E e, FmtContext& ctx) const
  {
    return fmt::formatter<string_view>::format(enchantum::details::format(e), ctx);
  }
};