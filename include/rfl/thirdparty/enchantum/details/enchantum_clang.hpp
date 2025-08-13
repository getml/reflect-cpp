#pragma once
#if __clang_major__ < 20
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wenum-constexpr-conversion"
#endif

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

#if __clang_major__ >= 20

  template<typename T, auto V, typename = void>
  inline constexpr bool is_valid_cast = false;

  template<typename T, auto V>
  inline constexpr bool is_valid_cast<T, V, std::void_t<std::integral_constant<T, static_cast<T>(V)>>> = true;

  template<typename T, std::underlying_type_t<T> max_range = 1>
  constexpr auto valid_cast_range()
  {
    if constexpr (max_range >= 0) {
      if constexpr (max_range <= ENCHANTUM_MAX_RANGE) {
        // this tests whether `static_cast`ing max_range is valid
        // because C style enums stupidly is like a bit field
        // `enum E { a,b,c,d = 3};` is like a bitfield `struct E { int val : 2;}`
        // which means giving E.val a larger than 2 bit value is UB so is it for enums
        // and gcc and msvc ignore this (for good)
        // while clang makes it a subsituation failure which we can check for
        // using std::inegral_constant makes sure this is a constant expression situation
        // for SFINAE to occur
        if constexpr (is_valid_cast<T, max_range>)
          return valid_cast_range<T, max_range * 2>();
        else
          return max_range - 1;
      }
      else {
        return max_range - 1;
      }
    }
    else {
      if constexpr (max_range >= ENCHANTUM_MIN_RANGE) {
        // this tests whether `static_cast`ing max_range is valid
        // because C style enums stupidly is like a bit field
        // `enum E { a,b,c,d = 3};` is like a bitfield `struct E { int val : 2;}`
        // which means giving E.val a larger than 2 bit value is UB so is it for enums
        // and gcc and msvc ignore this (for good)
        // while clang makes it a subsituation failure which we can check for
        // using std::inegral_constant makes sure this is a constant expression situation
        // for SFINAE to occur
        if constexpr (is_valid_cast<T, max_range>)
          return valid_cast_range<T, max_range * 2>();
        else
          return max_range / 2;
      }
      else {
        return max_range / 2;
      }
    }
  }
#else
  template<typename T, std::underlying_type_t<T> max_range = 1>
  constexpr auto valid_cast_range()
  {
    if constexpr (max_range >= 0)
      return ENCHANTUM_MAX_RANGE;
    else
      return ENCHANTUM_MIN_RANGE;
  }
#endif

} // namespace details

template<UnscopedEnum E>
  requires SignedEnum<E> && (!EnumFixedUnderlying<E>)
struct enum_traits<E> {
  static constexpr auto          max = details::valid_cast_range<E>();
  static constexpr decltype(max) min = details::valid_cast_range<E, -1>();
};

template<UnscopedEnum E>
  requires UnsignedEnum<E> && (!EnumFixedUnderlying<E>)
struct enum_traits<E> {
  static constexpr auto          max = details::valid_cast_range<E>();
  static constexpr decltype(max) min = 0;
};

namespace details {

  template<typename _>
  constexpr auto type_name_func() noexcept
  {
    // constexpr auto f() [with _ = Scoped]
    //return __PRETTY_FUNCTION__;
    constexpr auto funcname = string_view(
      __PRETTY_FUNCTION__ + (sizeof("auto enchantum::details::type_name_func() [_ = ") - 1));
    // (sizeof("auto __cdecl enchantum::details::type_name_func<") - 1)
    constexpr auto         size = funcname.size() - (sizeof("]") - 1);
    std::array<char, size> ret;
    auto* const            ret_data      = ret.data();
    const auto* const      funcname_data = funcname.data();
    for (std::size_t i = 0; i < size; ++i)
      ret_data[i] = funcname_data[i];
    return ret;
  }

  template<typename T>
  inline constexpr auto type_name = type_name_func<T>();


  template<auto Enum>
  constexpr auto enum_in_array_name() noexcept
  {
    // constexpr auto f() [with auto _ = (
    //constexpr auto f() [Enum = (Scoped)0]
    string_view s = __PRETTY_FUNCTION__ + (sizeof("auto enchantum::details::enum_in_array_name() [Enum = ") - 1);
    s.remove_suffix(sizeof("]") - 1);

    if constexpr (ScopedEnum<decltype(Enum)>) {
      if (s[s.size() - 2] == ')') {
        s.remove_prefix(sizeof("(") - 1);
        s.remove_suffix(sizeof(")0") - 1);
        return s;
      }
      else {
        return s.substr(0, s.rfind("::"));
      }
    }
    else {
      if (s[s.size() - 2] == ')') {
        s.remove_prefix(sizeof("(") - 1);
        s.remove_suffix(sizeof(")0") - 1);
      }
      if (const auto pos = s.rfind("::"); pos != s.npos)
        return s.substr(0, pos);
      return string_view();
    }
  }

  template<auto... Vs>
  constexpr auto var_name() noexcept
  {
    // "auto enchantum::details::var_name() [Vs = <(A)0, a, b, c, e, d, (A)6>]"
#define SZC(x) (sizeof(x) - 1)
    constexpr auto funcsig_off = SZC("auto enchantum::details::var_name() [Vs = <");
    return string_view(__PRETTY_FUNCTION__ + funcsig_off, SZC(__PRETTY_FUNCTION__) - funcsig_off - SZC(">]"));
#undef SZC
  }


  template<auto Copy>
  inline constexpr auto static_storage_for = Copy;

  template<typename E, typename Pair, bool ShouldNullTerminate>
  constexpr auto reflect() noexcept
  {
    constexpr auto Min = enum_traits<E>::min;
    constexpr auto Max = enum_traits<E>::max;

    constexpr auto elements = []() {
      constexpr auto Array = details::generate_arrays<E, Min, Max>();
      auto           str   = [Array]<std::size_t... Idx>(std::index_sequence<Idx...>) {
        return details::var_name<Array[Idx]...>();
      }(std::make_index_sequence<Array.size()>());

      struct RetVal {
        std::array<Pair, decltype(Array){}.size()> pairs{};
        std::size_t                                total_string_length = 0;
        std::size_t                                valid_count         = 0;
      } ret;

      std::size_t    index              = 0;
      constexpr auto enum_in_array_name = details::enum_in_array_name<E{}>();
      constexpr auto enum_in_array_len  = enum_in_array_name.size();

      // ((anonymous namespace)::A)0
      // (anonymous namespace)::a

      // this is needed to determine whether the above are cast expression if 2 braces are
      // next to eachother then it is a cast but only for anonymoused namespaced enums
      constexpr std::size_t index_check = !enum_in_array_name.empty() && enum_in_array_name.front() == '(' ? 1 : 0;
      while (index < Array.size()) {
        if (str[index_check] == '(') {
          str.remove_prefix(sizeof("(") - 1 + enum_in_array_len + sizeof(")0") - 1); // there is atleast 1 base 10 digit
          //if(!str.empty())
          //	std::cout << "after str \"" << str << '"' << '\n';
          if (const auto commapos = str.find(','); commapos != str.npos)
            str.remove_prefix(commapos + 2);
          //std::cout << "strsize \"" << str.size() << '"' << '\n';
        }
        else {
          if constexpr (enum_in_array_len != 0) {
            str.remove_prefix(enum_in_array_len + (sizeof("::") - 1));
          }
          if constexpr (details::prefix_length_or_zero<E> != 0) {
            str.remove_prefix(details::prefix_length_or_zero<E>);
          }
          const auto commapos = str.find(',');

          const auto name = str.substr(0, commapos);

          ret.pairs[ret.valid_count] = Pair{Array[index], name};
          ret.total_string_length += name.size() + ShouldNullTerminate;

          if (commapos != str.npos)
            str.remove_prefix(commapos + 2); // skip comma and space
          ++ret.valid_count;
        }
        ++index;
      }
      return ret;
    }();

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


//template<Enum E>
//constexpr std::size_t enum_count = details::enum_count<E>;


} // namespace enchantum

#if __clang_major__ < 20
  #pragma clang diagnostic pop
#endif
