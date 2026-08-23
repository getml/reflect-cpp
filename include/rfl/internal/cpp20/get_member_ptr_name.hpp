#ifndef RFL_INTERNAL_CPP20_GET_MEMBER_PTR_NAME_HPP_
#define RFL_INTERNAL_CPP20_GET_MEMBER_PTR_NAME_HPP_

#include <type_traits>
#include <utility>

#include "../StringLiteral.hpp"
#include "../is_field.hpp"
#include "../is_rename.hpp"
#include "../lit_name.hpp"

#if __has_include(<source_location>)
#include <source_location>
#endif

namespace rfl::internal::cpp20 {

template <class T, auto ptr>
consteval auto get_member_ptr_name_str_view() {
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
  return split.substr(split.find_last_of(":.") + 1);
#elif defined(__GNUC__)
  const auto split = func_name.substr(0, func_name.size() - 1);
  return split.substr(split.find_last_of(":.") + 1);
#elif defined(_MSC_VER)
  auto split = func_name.substr(0, func_name.size() - 7);
  auto pos = split.find_last_of(":");
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
struct member_type_helper;

template <class StructType, class PtrType>
struct member_type_helper<PtrType StructType::*> {
  using ptr_type = std::remove_cvref_t<PtrType>;
  using struct_type = std::remove_cvref_t<StructType>;
};

template <class T>
using struct_type_t = member_type_helper<T>::struct_type;

template <class T>
using ptr_type_t = member_type_helper<T>::ptr_type;

template <auto ptr>
consteval auto get_member_ptr_name() {
  using Type = ptr_type_t<decltype(ptr)>;
  if constexpr (is_rename_v<Type> || is_field_v<Type>) {
    using Name = typename Type::Name;
    return lit_name_v<Name>;
  } else {
    using StructType = struct_type_t<decltype(ptr)>;
    constexpr auto name = get_member_ptr_name_str_view<StructType, ptr>();
    return StringLiteral<name.size() + 1>(name);
  }
}

}  // namespace rfl::internal::cpp20

#endif
