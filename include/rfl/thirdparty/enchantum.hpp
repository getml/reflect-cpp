#pragma once

#ifndef ENCHANTUM_ALIAS_OPTIONAL
  #include <optional>
#endif

namespace enchantum {
#ifdef ENCHANTUM_ALIAS_OPTIONAL
ENCHANTUM_ALIAS_OPTIONAL;
#else
using ::std::optional;
#endif

} // namespace enchantum

#ifndef ENCHANTUM_ALIAS_STRING_VIEW
  #include <string_view>
#endif

namespace enchantum {
#ifdef ENCHANTUM_ALIAS_STRING_VIEW
ENCHANTUM_ALIAS_STRING_VIEW;
#else
using ::std::string_view;
#endif

} // namespace enchantum

#include <concepts>
#include <limits>
#include <string_view>
#include <type_traits>

#ifndef ENCHANTUM_ASSERT
  #include <cassert>
// clang-format off
  #define ENCHANTUM_ASSERT(cond, msg, ...) assert(cond && msg)
// clang-format on
#endif

#ifndef ENCHANTUM_THROW
  // additional info such as local variables are here
  #define ENCHANTUM_THROW(exception, ...) throw exception
#endif

#ifndef ENCHANTUM_MAX_RANGE
  #define ENCHANTUM_MAX_RANGE 256
#endif
#ifndef ENCHANTUM_MIN_RANGE
  #define ENCHANTUM_MIN_RANGE (-ENCHANTUM_MAX_RANGE)
#endif

namespace enchantum {

template<typename T>
concept Enum = std::is_enum_v<T>;

template<typename T>
concept SignedEnum = Enum<T> && std::signed_integral<std::underlying_type_t<T>>;

template<typename T>
concept UnsignedEnum = Enum<T> && !SignedEnum<T>;

template<typename T>
concept ScopedEnum = Enum<T> && (!std::is_convertible_v<T, std::underlying_type_t<T>>);

template<typename T>
concept UnscopedEnum = Enum<T> && !ScopedEnum<T>;

template<typename E, typename Underlying>
concept EnumOfUnderlying = Enum<E> && std::same_as<std::underlying_type_t<E>, Underlying>;

template<Enum E>
inline constexpr bool is_bitflag = requires(E e) {
  requires std::same_as<decltype(e & e), bool> || std::same_as<decltype(e & e), E>;
  { ~e } -> std::same_as<E>;
  { e | e } -> std::same_as<E>;
  { e &= e } -> std::same_as<E&>;
  { e |= e } -> std::same_as<E&>;
};

template<typename T>
concept BitFlagEnum = Enum<T> && is_bitflag<T>;

template<typename T>
concept EnumFixedUnderlying = Enum<T> && requires { T{0}; };

template<typename T>
struct enum_traits;

template<SignedEnum E>
struct enum_traits<E> {
private:
  using U = std::underlying_type_t<E>;
  using L = std::numeric_limits<U>;
public:
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = (L::min)() > ENCHANTUM_MIN_RANGE ? (L::min)() : ENCHANTUM_MIN_RANGE;
  static constexpr auto max = (L::max)() < ENCHANTUM_MAX_RANGE ? (L::max)() : ENCHANTUM_MAX_RANGE;
};

template<UnsignedEnum E>
struct enum_traits<E> {
private:
  using T = std::underlying_type_t<E>;
  using L = std::numeric_limits<T>;
public:
  static constexpr std::size_t prefix_length = 0;

  static constexpr auto min = []() {
    if constexpr (std::is_same_v<T, bool>)
      return false;
    else
      return (ENCHANTUM_MIN_RANGE) < 0 ? 0 : (ENCHANTUM_MIN_RANGE);
  }();
  static constexpr auto max = []() {
    if constexpr (std::is_same_v<T, bool>)
      return true;
    else
      return (L::max)() < (ENCHANTUM_MAX_RANGE) ? (L::max)() : (ENCHANTUM_MAX_RANGE);
  }();
};

} // namespace enchantum
#include <array>
#include <climits>
#include <cstddef>
#include <type_traits>

namespace enchantum::details {

template<typename E, typename = void>
inline constexpr std::size_t prefix_length_or_zero = 0;

template<typename E>
inline constexpr auto prefix_length_or_zero<E, decltype((void)enum_traits<E>::prefix_length)> = std::size_t{
  enum_traits<E>::prefix_length};

template<typename Enum, auto Min, decltype(Min) Max>
constexpr auto generate_arrays()
{
#if defined __clang__ && __clang_major__ >= 20
  if constexpr (BitFlagEnum<Enum>) {
    if constexpr (EnumFixedUnderlying<Enum>) {
      constexpr std::size_t      bits = sizeof(Enum) * CHAR_BIT;
      std::array<Enum, bits + 1> ret{}; // 0 value reflected
      for (std::size_t i = 0; i < bits; ++i)
        ret[i + 1] = static_cast<Enum>(static_cast<std::make_unsigned_t<std::underlying_type_t<Enum>>>(1) << i);
      return ret;
    }
    else {
      constexpr auto bits =
        []() {
          auto        copy  = (Max > Min ? Max - Min : Min - Max); // handle negative;
          std::size_t count = 0;
          do
            ++count;
          while (copy >>= 1);
          return count;
        }();
      std::array<Enum, bits + 1> b{}; // 0 value reflected
      for (std::size_t i = 0; i < bits; ++i)
        b[i + 1] = static_cast<Enum>(static_cast<std::make_unsigned_t<std::underlying_type_t<Enum>>>(1) << i);
      return b;
    }
  }
#else
  if constexpr (BitFlagEnum<Enum>) {
    constexpr std::size_t      bits = sizeof(Enum) * CHAR_BIT;
    std::array<Enum, bits + 1> ret{}; // 0 value reflected
    for (std::size_t i = 0; i < bits; ++i)
      ret[i + 1] = static_cast<Enum>(static_cast<std::make_unsigned_t<std::underlying_type_t<Enum>>>(1) << i);
    return ret;
  }
#endif
  else {
    static_assert(Min < Max, "enum_traits::min must be less than enum_traits::max");
    std::array<Enum, (Max - Min) + 1> array;
    auto* const                       array_data = array.data();
    for (std::size_t i = 0, size = array.size(); i < size; ++i)
      array_data[i] = static_cast<Enum>(static_cast<decltype(Min)>(i) + Min);
    return array;
  }
}
} // namespace enchantum::details

#if defined(__clang__)
  
#if __clang_major__ < 20
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wenum-constexpr-conversion"
#endif

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

#elif defined(__GNUC__) || defined(__GNUG__)
  

#include <array>
#include <cassert>
#include <climits>
#include <type_traits>
#include <utility>

namespace enchantum {

namespace details {

  template<typename _>
  constexpr auto type_name_func() noexcept
  {
    // constexpr auto f() [with _ = Scoped]
    //return __PRETTY_FUNCTION__;
    constexpr auto funcname = string_view(
      __PRETTY_FUNCTION__ + (sizeof("constexpr auto enchantum::details::type_name_func() [with _ = ") - 1));
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
    //constexpr auto f() [with auto _ = (Scoped)0]
    string_view s = __PRETTY_FUNCTION__ +
      sizeof("constexpr auto enchantum::details::enum_in_array_name() [with auto Enum = ") - 1;
    s.remove_suffix(sizeof("]") - 1);

    if constexpr (ScopedEnum<decltype(Enum)>) {
      if (s.front() == '(') {
        s.remove_prefix(1);
        s.remove_suffix(sizeof(")0") - 1);
        return s;
      }
      else {
        return s.substr(0, s.rfind("::"));
      }
    }
    else {
      if (s.front() == '(') {
        s.remove_prefix(1);
        s.remove_suffix(sizeof(")0") - 1);
      }
      if (const auto pos = s.rfind("::"); pos != s.npos)
        return s.substr(0, pos);
      return string_view();
    }
  }

  template<typename Enum>
  constexpr auto length_of_enum_in_template_array_if_casting() noexcept
  {
    if constexpr (ScopedEnum<Enum>) {
      return details::enum_in_array_name<Enum{}>().size();
    }
    else {
      constexpr auto  s      = enum_in_array_name<Enum{}>().size();
      constexpr auto& tyname = type_name<Enum>;
      constexpr auto  str    = string_view(tyname.data(), tyname.size());
      if (constexpr auto pos = str.rfind("::"); pos != str.npos) {
        return s + str.substr(pos).size();
      }
      else {
        return s + tyname.size();
      }
    }
  }

  template<auto... Vs>
  constexpr auto var_name() noexcept
  {
    //constexpr auto f() [with auto _ = std::array<E, 6>{std::__array_traits<E, 6>::_Type{a, b, c, e, d, (E)6}}]
#define SZC(x) (sizeof(x) - 1)
    constexpr std::size_t funcsig_off = SZC("constexpr auto enchantum::details::var_name() [with auto ...Vs = {");
    return std::string_view(__PRETTY_FUNCTION__ + funcsig_off, SZC(__PRETTY_FUNCTION__) - funcsig_off - SZC("}]"));
  }

  template<auto Copy>
  inline constexpr auto static_storage_for = Copy;

  template<typename E, typename Pair, bool ShouldNullTerminate>
  constexpr auto reflect() noexcept
  {
    constexpr auto Min      = enum_traits<E>::min;
    constexpr auto Max      = enum_traits<E>::max;

    constexpr auto elements = []() {
      constexpr auto length_of_enum_in_template_array_casting = details::length_of_enum_in_template_array_if_casting<E>();
      constexpr auto Array = details::generate_arrays<E, Min, Max>();
      auto           str   = [Array]<std::size_t... Idx>(std::index_sequence<Idx...>) {
        return details::var_name<Array[Idx]...>();
      }(std::make_index_sequence<Array.size()>());
      struct RetVal {
        std::array<Pair, Array.size()> pairs{};
        std::size_t                    total_string_length = 0;
        std::size_t                    valid_count         = 0;
      } ret;
      std::size_t    index             = 0;
      constexpr auto enum_in_array_len = enum_in_array_name<E{}>().size();
      while (index < Array.size()) {
        if (str.front() == '(') {
          str.remove_prefix(sizeof("(") - 1 + length_of_enum_in_template_array_casting + sizeof(")0") -
                            1); // there is atleast 1 base 10 digit
          //if(!str.empty())
          //	std::cout << "after str \"" << str << '"' << '\n';

          if (const auto commapos = str.find(','); commapos != str.npos)
            str.remove_prefix(commapos + 2);

          //std::cout << "strsize \"" << str.size() << '"' << '\n';
        }
        else {
          if constexpr (enum_in_array_len != 0)
            str.remove_prefix(enum_in_array_len + sizeof("::") - 1);
          if constexpr (details::prefix_length_or_zero<E> != 0) {
            str.remove_prefix(details::prefix_length_or_zero<E>);
          }
          const auto commapos = str.find(',');

          const auto name = str.substr(0, commapos);

          ret.pairs[ret.valid_count] = Pair{Array[index], name};
          ret.total_string_length += name.size() + ShouldNullTerminate;

          if (commapos != str.npos)
            str.remove_prefix(commapos + 2);
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

} // namespace enchantum
#elif defined(_MSC_VER)
  

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

#endif

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
#include <type_traits>
#include <utility>

namespace enchantum {

template<typename>
inline constexpr bool has_zero_flag = false;

template<BitFlagEnum E>
inline constexpr bool has_zero_flag<E> = []() {
  for (const auto v : values<E>)
    if (static_cast<std::underlying_type_t<E>>(v) == 0)
      return true;
  return false;
}();

template<typename>
inline constexpr bool is_contiguous = false;

template<Enum E>
inline constexpr bool is_contiguous<E> = []() {
  using T = std::underlying_type_t<E>;
  if constexpr (std::is_same_v<T, bool>) {
    return true;
  }
  else {
    constexpr auto& enums = entries<E>;
    for (std::size_t i = 0; i < enums.size() - 1; ++i)
      if (T(enums[i].first) + 1 != T(enums[i + 1].first))
        return false;
    return true;
  }
}();

template<typename E>
concept ContiguousEnum = Enum<E> && is_contiguous<E>;

template<typename>
inline constexpr bool is_contiguous_bitflag = false;

template<BitFlagEnum E>
inline constexpr bool is_contiguous_bitflag<E> = []() {
  constexpr auto& enums = entries<E>;
  using T               = std::underlying_type_t<E>;
  for (auto i = std::size_t{has_zero_flag<E>}; i < enums.size() - 1; ++i)
    if (T(enums[i].first) << 1 != T(enums[i + 1].first))
      return false;
  return true;
}();

template<typename E>
concept ContiguousBitFlagEnum = BitFlagEnum<E> && is_contiguous_bitflag<E>;

template<Enum E>
[[nodiscard]] constexpr bool contains(const E value) noexcept
{
  for (const auto v : values<E>)
    if (v == value)
      return true;
  return false;
}

template<Enum E>
[[nodiscard]] constexpr bool contains(const std::underlying_type_t<E> value) noexcept
{
  return enchantum::contains(static_cast<E>(value));
}

template<Enum E>
[[nodiscard]] constexpr bool contains(const string_view name) noexcept
{
  for (const auto& s : names<E>)
    if (s == name)
      return true;
  return false;
}

template<Enum E, std::predicate<string_view, string_view> BinaryPredicate>
[[nodiscard]] constexpr bool contains(const string_view name, const BinaryPredicate binary_predicate) noexcept
{
  for (const auto& s : names<E>)
    if (binary_predicate(name, s))
      return true;
  return false;
}

template<ContiguousEnum E>
[[nodiscard]] constexpr bool contains(const E value) noexcept
{
  using T = std::underlying_type_t<E>;
  return T(value) <= T(max<E>) && T(value) >= T(min<E>);
}

namespace details {
  template<typename E>
  struct index_to_enum_functor {
    [[nodiscard]] constexpr optional<E> operator()(const std::size_t index) const noexcept
    {
      optional<E> ret;
      if (index < values<E>.size())
        ret.emplace(values<E>[index]);
      return ret;
    }
  };

  struct enum_to_index_functor {
    template<Enum E>
    [[nodiscard]] constexpr optional<std::size_t> operator()(const E e) const noexcept
    {
      if constexpr (ContiguousEnum<E>) {
        using T = std::underlying_type_t<E>;
        if (enchantum::contains(e))
          return optional<std::size_t>(std::size_t(T(e) - T(min<E>)));
      }
      else {
        for (std::size_t i = 0; i < values<E>.size(); ++i)
          if (values<E>[i] == e)
            return optional<std::size_t>(i);
      }
      return optional<std::size_t>();
    }
  };

  template<Enum E>
  struct cast_functor {
    [[nodiscard]] constexpr optional<E> operator()(const std::underlying_type_t<E> value) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      if (!enchantum::contains<E>(value))
        return a;
      a.emplace(static_cast<E>(value));
      return a;
    }

    [[nodiscard]] constexpr optional<E> operator()(const string_view name) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      for (const auto& [e, s] : entries<E>) {
        if (s == name) {
          a.emplace(e);
          return a;
        }
      }
      return a; // nullopt
    }

    template<std::predicate<string_view, string_view> BinaryPred>
    [[nodiscard]] constexpr optional<E> operator()(const string_view name, const BinaryPred binary_predicate) const noexcept
    {
      optional<E> a; // rvo not that it really matters
      for (const auto& [e, s] : entries<E>) {
        if (binary_predicate(name, s)) {
          a.emplace(e);
          return a;
        }
      }
      return a;
    }
  };

} // namespace details

template<Enum E>
inline constexpr details::index_to_enum_functor<E> index_to_enum{};

inline constexpr details::enum_to_index_functor enum_to_index{};

template<Enum E>
inline constexpr details::cast_functor<E> cast{};

namespace details {
  struct to_string_functor {
    template<Enum E>
    [[nodiscard]] constexpr string_view operator()(const E value) const noexcept
    {
      if (const auto i = enchantum::enum_to_index(value))
        return names<E>[*i];
      return string_view();
    }
  };

} // namespace details
inline constexpr details::to_string_functor to_string{};

} // namespace enchantum
#include <concepts>
#include <utility>

namespace enchantum {

#if 0
namespace details {

  template<std::size_t range, std::size_t sets>
  constexpr auto cartesian_product()
  {
    constexpr auto size = []() {
      std::size_t x = range;
      std::size_t n = sets;
      while (--n != 0)
        x *= range;
      return x;
    }();

    std::array<std::array<std::size_t, sets>, size> products{};
    std::array<std::size_t, sets>                   counter{};

    for (auto& product : products) {
      product = counter;

      ++counter.back();
      for (std::size_t i = counter.size() - 1; i != 0; i--) {
        if (counter[i] != range)
          break;

        counter[i] = 0;
        ++counter[i - 1];
      }
    }
    return products;
  }

} // namespace details
#endif

#if 0
template<Enum E, std::invocable<E> Func>
constexpr auto visit(Func func, E e) 
noexcept(std::is_nothrow_invocable_v<Func, E>)
{
  using Ret = decltype(func(e));
  

  return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
    if ((values<Enums>[Idx] == enums))
      (func(std::integral_constant<E, values<E>[Idx]> {}), ...);
  }(std::make_index_sequence<count<E>>());
}
template<Enum... Enums, std::invocable<Enums...> Func>
constexpr auto visit(Func func, Enums... enums) noexcept(std::is_nothrow_invocable_v<Func, Enums...>)
{
  using Ret = decltype(func(enums...));
  return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
    if ((values<Enums>[Idx] == enums) && ...)
      (func(std::integral_constant<E, values<E>[Idx]> {}), ...);
  }(std::make_index_sequence<count<Enums>>()...);
}
#endif
namespace details {

  template<typename E, typename Func, std::size_t... I>
  constexpr auto for_each(Func& f, std::index_sequence<I...>)
  {
    (void)(f(std::integral_constant<E, values<E>[I]> {}), ...);
  }

} // namespace details

template<Enum E, typename Func>
constexpr void for_each(Func f) // intentional not const
{
  details::for_each<E>(f, std::make_index_sequence<count<E>>{});
}
} // namespace enchantum

#include <array>
#include <stdexcept>

namespace enchantum {

template<Enum E, typename V>
class array : public std::array<V, count<E>> {
private:
  using base = std::array<V, count<E>>;
public:
  using index_type = E;
  using typename base::const_iterator;
  using typename base::const_pointer;
  using typename base::const_reference;
  using typename base::const_reverse_iterator;
  using typename base::difference_type;
  using typename base::iterator;
  using typename base::pointer;
  using typename base::reference;
  using typename base::reverse_iterator;
  using typename base::size_type;
  using typename base::value_type;

  using base::at;
  using base::operator[];

  [[nodiscard]] constexpr reference at(const E index)
  {
    if (const auto i = enchantum::enum_to_index(index))
      return operator[](*i);
    ENCHANTUM_THROW(std::out_of_range("enchantum::array::at index out of range"), index);
  }

  [[nodiscard]] constexpr const_reference at(const E index) const
  {
    if (const auto i = enchantum::enum_to_index(index))
      return operator[](*i);
    ENCHANTUM_THROW(std::out_of_range("enchantum::array::at: index out of range"), index);
  }

  [[nodiscard]] constexpr reference operator[](const E index) noexcept
  {
    return operator[](*enchantum::enum_to_index(index));
  }

  [[nodiscard]] constexpr const_reference operator[](const E index) const noexcept
  {
    return operator[](*enchantum::enum_to_index(index));
  }
};

} // namespace enchantum

#ifndef ENCHANTUM_ALIAS_STRING
  #include <string>
#endif

namespace enchantum {
#ifdef ENCHANTUM_ALIAS_STRING
ENCHANTUM_ALIAS_STRING;
#else
using ::std::string;
#endif
} // namespace enchantum

namespace enchantum {

template<BitFlagEnum E>
inline constexpr E values_ors = [] {
  E ret{};
  for (const auto val : values<E>)
    ret |= val;
  return ret;
}();

template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const std::underlying_type_t<E> value) noexcept
{
  if (value == 0)
    return has_zero_flag<E>;
  using T      = std::underlying_type_t<E>;
  T valid_bits = 0;

  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    const auto v = static_cast<T>(values<E>[i]);
    if ((value & v) == v)
      valid_bits |= v;
  }
  return valid_bits == value;
}

template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const E value) noexcept
{
  return enchantum::contains_bitflag<E>(static_cast<std::underlying_type_t<E>>(value));
}

template<BitFlagEnum E, std::predicate<string_view, string_view> BinaryPred>
[[nodiscard]] constexpr bool contains_bitflag(const string_view s, const char sep, const BinaryPred binary_pred) noexcept
{
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (!enchantum::contains<E>(s.substr(pos, i - pos)), binary_pred)
      return false;
    pos = i + 1;
  }
  return enchantum::contains<E>(s.substr(pos), binary_pred);
}

template<BitFlagEnum E>
[[nodiscard]] constexpr bool contains_bitflag(const string_view s, const char sep = '|') noexcept
{
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (!enchantum::contains<E>(s.substr(pos, i - pos)))
      return false;
    pos = i + 1;
  }
  return enchantum::contains<E>(s.substr(pos));
}

template<typename String = string, BitFlagEnum E>
[[nodiscard]] constexpr String to_string_bitflag(const E value, const char sep = '|')
{
  using T = std::underlying_type_t<E>;
  if constexpr (has_zero_flag<E>)
    if (static_cast<T>(value) == 0)
      return String(names<E>[0]);

  String name;
  T      check_value = 0;
  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    const auto& [v, s] = entries<E>[i];
    if (v == (value & v)) {
      if (!name.empty())
        name.append(1, sep);           // append separator if not the first value
      name.append(s.data(), s.size()); // not using operator += since this may not be std::string_view always
      check_value |= static_cast<T>(v);
    }
  }
  if (check_value == static_cast<T>(value))
    return name;
  return String{};
}
 
template<BitFlagEnum E, std::predicate<string_view, string_view> BinaryPred>
[[nodiscard]] constexpr optional<E> cast_bitflag(const string_view s, const char sep, const BinaryPred binary_pred) noexcept
{
  using T = std::underlying_type_t<E>;
  T           check_value{};
  std::size_t pos = 0;
  for (std::size_t i = s.find(sep); i != s.npos; i = s.find(sep, pos)) {
    if (const auto v = enchantum::cast<E>(s.substr(pos, i - pos),binary_pred))
      check_value |= static_cast<T>(*v);
    else
      return optional<E>();
    pos = i + 1;
  }

  if (const auto v = enchantum::cast<E>(s.substr(pos), binary_pred))
    return optional<E>(static_cast<E>(check_value | static_cast<T>(*v)));
  return optional<E>();
}

template<BitFlagEnum E>
[[nodiscard]] constexpr optional<E> cast_bitflag(const string_view s, const char sep = '|') noexcept
{
  return enchantum::cast_bitflag<E>(s, sep, [](const auto& a, const auto& b) { return a == b; });
}

template<BitFlagEnum E>
[[nodiscard]] constexpr optional<E> cast_bitflag(const E value) noexcept
{
  using T              = std::underlying_type_t<E>;
  const auto raw_value = static_cast<T>(value);

  if constexpr (has_zero_flag<E>)
    if (raw_value == 0)
      return optional<E>(E{});

  T valid_bits{0};
  for (auto i = std::size_t{has_zero_flag<E>}; i < count<E>; ++i) {
    auto v = static_cast<T>(values<E>[i]);
    if ((raw_value & v) == v)
      valid_bits |= v;
  }
  return valid_bits == raw_value ? optional<E>(value) : optional<E>();
}

} // namespace enchantum

#include <type_traits>
/*
Note this header is an extremely easy way to cause ODR issues.

class Flags { F1 = 1 << 0,F2 = 1<< 1};
// **note I did not define any operators**

enchantum::contains(Flags::F1); // considered a classical `Enum` concept 

using namespace enchantum::bitwise_operators;

enchantum::contains(Flags::F1); // considered `BitFlagEnum` concept woops! ODR! 

*/

namespace enchantum::bitwise_operators {
template<Enum E>
[[nodiscard]] constexpr E operator~(E e) noexcept
{
  return static_cast<E>(~static_cast<std::underlying_type_t<E>>(e));
}

template<Enum E>
[[nodiscard]] constexpr E operator|(E a, E b) noexcept
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(a) | static_cast<T>(b));
}

template<Enum E>
[[nodiscard]] constexpr E operator&(E a, E b) noexcept
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(a) & static_cast<T>(b));
}

template<Enum E>
[[nodiscard]] constexpr E operator^(E a, E b) noexcept
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(a) ^ static_cast<T>(b));
}

template<Enum E>
constexpr E& operator|=(E& a, E b) noexcept
{
  using T  = std::underlying_type_t<E>;
  return a = static_cast<E>(static_cast<T>(a) | static_cast<T>(b));
}

template<Enum E>
constexpr E& operator&=(E& a, E b) noexcept
{
  using T  = std::underlying_type_t<E>;
  return a = static_cast<E>(static_cast<T>(a) & static_cast<T>(b));
}

template<Enum E>
constexpr E& operator^=(E& a, E b) noexcept
{
  using T  = std::underlying_type_t<E>;
  return a = static_cast<E>(static_cast<T>(a) ^ static_cast<T>(b));
}

} // namespace enchantum::bitwise_operators

#define ENCHANTUM_DEFINE_BITWISE_FOR(Enum)                                                \
  [[nodiscard]] constexpr Enum operator&(Enum a, Enum b) noexcept                         \
  {                                                                                       \
    using T = std::underlying_type_t<Enum>;                                               \
    return static_cast<Enum>(static_cast<T>(a) & static_cast<T>(b));                      \
  }                                                                                       \
  [[nodiscard]] constexpr Enum operator|(Enum a, Enum b) noexcept                         \
  {                                                                                       \
    using T = std::underlying_type_t<Enum>;                                               \
    return static_cast<Enum>(static_cast<T>(a) | static_cast<T>(b));                      \
  }                                                                                       \
  [[nodiscard]] constexpr Enum operator^(Enum a, Enum b) noexcept                         \
  {                                                                                       \
    using T = std::underlying_type_t<Enum>;                                               \
    return static_cast<Enum>(static_cast<T>(a) ^ static_cast<T>(b));                      \
  }                                                                                       \
  constexpr Enum&              operator&=(Enum& a, Enum b) noexcept { return a = a & b; } \
  constexpr Enum&              operator|=(Enum& a, Enum b) noexcept { return a = a | b; } \
  constexpr Enum&              operator^=(Enum& a, Enum b) noexcept { return a = a ^ b; } \
  [[nodiscard]] constexpr Enum operator~(Enum a) noexcept                                 \
  {                                                                                       \
    return static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(a));              \
  }

#include <iosfwd>
#include <string>
#include <concepts>

namespace enchantum::istream_operators {
template<typename Traits, Enum E>
  requires std::assignable_from<E&,E>
std::basic_istream<char, Traits>& operator>>(std::basic_istream<char, Traits>& is, E& value)
{
  std::basic_string<char, Traits> s;
  is >> s;
  if (!is)
    return is;

  if (const auto v = enchantum::cast<E>(s))
    value = *v;
  else
    is.setstate(std::ios_base::failbit);
  return is;
}

} // namespace enchantum::istream_operators

#include <iosfwd>
#include <string_view>

namespace enchantum::ostream_operators {
template<typename Traits, Enum E>
std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const E value)
{
  return os << enchantum::to_string(value);
}
} // namespace enchantum::ostream_operators

namespace enchantum::iostream_operators {
using ::enchantum::istream_operators::operator>>;
using ::enchantum::ostream_operators::operator<<;
} // namespace enchantum::iostream_operators

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

#if __has_include(<fmt/format.h>)
  

#include <fmt/format.h>
#include <string_view>

template<enchantum::Enum E>
struct fmt::formatter<E> {
  template<typename ParseContext>
  static constexpr auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template<typename FmtContext>
  static auto format(const E e, FmtContext& ctx)
  {
    if constexpr (enchantum::BitFlagEnum<E>)
      return fmt::format_to(ctx.out(), "{}", enchantum::to_string_bitflag(e));
    else
      return fmt::format_to(ctx.out(), "{}", enchantum::to_string(e));
  }
};
#elif __has_include(<format>)
  

#include <format>
#include <string_view>

template<enchantum::Enum E>
struct std::formatter<E> {
  template<typename ParseContext>
  static constexpr auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template<typename FmtContext>
  static auto format(const E e, FmtContext& ctx)
  {
    if constexpr (enchantum::BitFlagEnum<E>)
      return std::format_to(ctx.out(), "{}", enchantum::to_string_bitflag(e));
    else
      return std::format_to(ctx.out(), "{}", enchantum::to_string(e));
  }
};
#endif