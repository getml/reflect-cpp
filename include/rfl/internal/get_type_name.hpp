#ifndef RFL_INTERNAL_GETTYPENAME_HPP_
#define RFL_INTERNAL_GETTYPENAME_HPP_

#include <utility>

#include "StringLiteral.hpp"

#if __has_include(<source_location>)
#include <source_location>
#endif

namespace rfl {
namespace internal {

template <class T>
consteval auto get_type_name_str_view() {
#if __cpp_lib_source_location >= 201907L
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
#elif defined(_MSC_VER)
  // Officially, we only support MSVC versions that are modern enough to contain
  // <source_location>, but inofficially, this might work.
  const auto func_name = std::string_view{__FUNCSIG__};
#else
  const auto func_name = std::string_view{__PRETTY_FUNCTION__};
#endif
#if defined(__clang__)
  const auto split = func_name.substr(0, func_name.size() - 1);
  return split.substr(split.find("T = ") + 4);
#elif defined(__GNUC__)
  const auto split = func_name.substr(0, func_name.size() - 1);
  return split.substr(split.find("T = ") + 4);
#elif defined(_MSC_VER)
  auto split = func_name.substr(0, func_name.size() - 7);
  split = split.substr(split.find("get_type_name_str_view<") + 23);
  auto pos = split.find(" ");
  if (pos != std::string_view::npos) {
    return split.substr(pos + 1);
  }
  return split;
#else
  static_assert(
      false,
      "You are using an unsupported compiler. Please use GCC, Clang "
      "or MSVC or explicitly tag your structs using 'Tag' or 'Name'.");
#endif
}

template <class T>
consteval auto   get_type_name() {
  static_assert(get_type_name_str_view<int>() == "int",
                "Expected 'int', got something else.");
  constexpr auto name = get_type_name_str_view<T>();
  const auto to_str_lit = [&]<auto... Ns>(std::index_sequence<Ns...>) {
    return StringLiteral<sizeof...(Ns) + 1>{name[Ns]...};
  };
  return to_str_lit(std::make_index_sequence<name.size()>{});
}

}  // namespace internal
}  // namespace rfl

#endif
