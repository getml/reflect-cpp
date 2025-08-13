#pragma once
#include "../common.hpp"
#include "generate_arrays.hpp"
#include "string_view.hpp"
#include <array>
#include <cassert>
#include <climits>
#include <type_traits>
#include <utility>

namespace enchantum {

namespace details {

#define SZC(x) (sizeof(x) - 1)
  template<typename>
  constexpr auto type_name_func_size() noexcept
  {
    // (sizeof("auto __cdecl enchantum::details::type_name_func<") - 1)
    return SZC(__FUNCSIG__) - SZC("auto __cdecl enchantum::details::type_name_func_size<enum ") -
      SZC(">(void) noexcept");
  }

  template<auto Enum>
  constexpr auto enum_in_array_name() noexcept
  {
    string_view s = __FUNCSIG__ + sizeof("auto __cdecl enchantum::details::enum_in_array_name<") - 1;
    s.remove_suffix(sizeof(">(void) noexcept") - 1);

    if constexpr (ScopedEnum<decltype(Enum)>) {
      if (s.front() == '(') {
        s.remove_prefix(sizeof("(enum ") - 1);
        s.remove_suffix(sizeof(")0x0") - 1);
        return s;
      }
      return s.substr(0, s.rfind("::"));
    }
    else {
      if (s.front() == '(') {
        s.remove_prefix(sizeof("(enum ") - 1);
        s.remove_suffix(sizeof(")0x0") - 1);
      }
      if (const auto pos = s.rfind("::"); pos != s.npos)
        return s.substr(0, pos);
      return string_view();
    }
  }

  template<auto Array>
  constexpr auto var_name() noexcept
  {
    //auto __cdecl f<class std::array<enum `anonymous namespace'::UnscopedAnon,32>{enum `anonymous-namespace'::UnscopedAnon

    using T = typename decltype(Array)::value_type;
#define SZC(x) (sizeof(x) - 1)
    std::size_t    funcsig_off   = SZC("auto __cdecl enchantum::details::var_name<class std::array<enum ");
    constexpr auto type_name_len = enchantum::details::type_name_func_size<T>();
    funcsig_off += type_name_len + SZC(",");
    constexpr auto Size = Array.size();
    // clang-format off
    funcsig_off += Size < 10 ? 1
    : Size < 100 ? 2
    : Size < 1000 ? 3
    : Size < 10000 ? 4
    : Size < 100000 ? 5
    : Size < 1000000 ? 6
    : Size < 10000000 ? 7
    : Size < 100000000 ? 8
    : Size < 1000000000 ? 9
    : 10;
    // clang-format on
    funcsig_off += SZC(">{enum ") + type_name_len;
    return string_view(__FUNCSIG__ + funcsig_off, SZC(__FUNCSIG__) - funcsig_off - (sizeof("}>(void) noexcept") - 1));
  }
#undef SZC


  template<auto Copy>
  inline constexpr auto static_storage_for = Copy;

  template<typename E, typename Pair, auto Array, bool ShouldNullTerminate>
  constexpr auto get_elements()
  {
    constexpr auto type_name_len = details::type_name_func_size<E>();

    auto str = var_name<Array>();
    struct RetVal {
      std::array<Pair, Array.size()> pairs{};
      std::size_t                    total_string_length = 0;
      std::size_t                    valid_count         = 0;
    } ret;
    std::size_t    index             = 0;
    constexpr auto enum_in_array_len = details::enum_in_array_name<E{}>().size();
    while (index < Array.size()) {
      if (str.front() == '(') {
        str.remove_prefix(sizeof("(enum ") - 1 + type_name_len + sizeof(")0x0") - 1); // there is atleast 1 base 16 hex digit

        if (const auto commapos = str.find(','); commapos != str.npos)
          str.remove_prefix(commapos + 1);
      }
      else {
        if constexpr (enum_in_array_len != 0)
          str.remove_prefix(enum_in_array_len + sizeof("::") - 1);

        if constexpr (details::prefix_length_or_zero<E> != 0)
          str.remove_prefix(details::prefix_length_or_zero<E>);

        const auto commapos = str.find(',');

        const auto name = str.substr(0, commapos);

        ret.pairs[ret.valid_count] = Pair{Array[index], name};
        ret.total_string_length += name.size() + ShouldNullTerminate;

        if (commapos != str.npos)
          str.remove_prefix(commapos + 1);
        ++ret.valid_count;
      }
      ++index;
    }
    return ret;
  }

  template<typename E, typename Pair, bool ShouldNullTerminate>
  constexpr auto reflect() noexcept
  {
    constexpr auto Min = enum_traits<E>::min;
    constexpr auto Max = enum_traits<E>::max;

    constexpr auto elements = details::get_elements<E, Pair, details::generate_arrays<E, Min, Max>(), ShouldNullTerminate>();

    constexpr auto strings = [elements]() {
      std::array<char, elements.total_string_length> strings;
      for (std::size_t _i = 0, index = 0; _i < elements.valid_count; ++_i) {
        const auto& [_, s] = elements.pairs[_i];
        for (std::size_t i = 0; i < s.size(); ++i)
          strings[index++] = s[i];

        if constexpr (ShouldNullTerminate)
          strings[index++] = '\0';
      }
      return strings;
    }();

    std::array<Pair, elements.valid_count> ret;
    constexpr const auto*                  str = static_storage_for<strings>.data();
    for (std::size_t i = 0, string_index = 0; i < elements.valid_count; ++i) {
      const auto& [e, s] = elements.pairs[i];
      auto& [re, rs]     = ret[i];
      re                 = e;

      rs = {str + string_index, str + string_index + s.size()};
      string_index += s.size() + ShouldNullTerminate;
    }

    return ret;
  }
} // namespace details


} // namespace enchantum
