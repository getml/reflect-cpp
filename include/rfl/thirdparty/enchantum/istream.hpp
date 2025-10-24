#pragma once
#include "enchantum.hpp"
#include <concepts>
#include <iosfwd>
#include <string>

namespace enchantum::istream_operators {
template<typename Traits, Enum E>
  requires std::assignable_from<E&, E>
std::basic_istream<char, Traits>& operator>>(std::basic_istream<char, Traits>& is, E& value)
{
  std::basic_string<char, Traits> s;
  is >> s;
  if (!is)
    return is;

  if constexpr (is_bitflag<E>) {
    if (const auto v = enchantum::cast_bitflag<E>(s))
      value = *v;
    else
      is.setstate(std::ios_base::failbit);
  }
  else {
    if (const auto v = enchantum::cast<E>(s))
      value = *v;
    else
      is.setstate(std::ios_base::failbit);
  }
  return is;
}

} // namespace enchantum::istream_operators
