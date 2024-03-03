#ifndef RFL_INTERNAL_GETTYPENAME_HPP_
#define RFL_INTERNAL_GETTYPENAME_HPP_

#include <source_location>
#include <string>
#include <utility>

namespace rfl {
namespace internal {

template <class T>
consteval auto get_type_name_str_view() {
  // Unfortunately, we cannot avoid the use of a compiler-specific macro for
  // Clang on Windows. For all other compilers, function_name works as intended.
#if defined(__clang__) && defined(_MSC_VER)
  const auto func_name = std::string_view{__PRETTY_FUNCTION__};
#else
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
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
consteval auto get_type_name() {
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
