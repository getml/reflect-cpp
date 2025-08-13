#pragma once

#include "enchantum.hpp"
#include <iosfwd>
#include <string_view>
#include "details/format_util.hpp"

namespace enchantum::ostream_operators {
template<typename Traits, Enum E>
std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const E e)
{
  return os << details::format(e);
}
} // namespace enchantum::ostream_operators