#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

#include <type_traits>
#include <utility>

#if __has_include(<source_location>)
#include <source_location>
#endif

#include "../Literal.hpp"
#include "get_ith_field_from_fake_object.hpp"
#include "is_flatten_field.hpp"
#include "is_rename.hpp"
#include "num_fields.hpp"

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

namespace rfl::internal {

template <class T>
struct Wrapper {
  using Type = T;
  T v;
};

template <class T>
Wrapper(T) -> Wrapper<T>;

// This workaround is necessary for clang.
template <class T>
constexpr auto wrap(const T& arg) noexcept {
  return Wrapper{arg};
}

template <class T, auto ptr>
consteval auto get_field_name_str_view() {
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
  const auto split = func_name.substr(0, func_name.size() - 2);
  return split.substr(split.find_last_of(":.") + 1);
#elif defined(__GNUC__)
  const auto split = func_name.substr(0, func_name.size() - 2);
  return split.substr(split.find_last_of(":") + 1);
#elif defined(_MSC_VER)
  const auto split = func_name.substr(0, func_name.size() - 7);
  return split.substr(split.rfind("->") + 2);
#else
  static_assert(false,
                "You are using an unsupported compiler. Please use GCC, Clang "
                "or MSVC or switch to the rfl::Field-syntax.");
#endif
}

template <class T, auto ptr>
consteval auto get_field_name_str_lit() {
  constexpr auto name = get_field_name_str_view<T, ptr>();
  const auto to_str_lit = [&]<auto... Ns>(std::index_sequence<Ns...>) {
    return StringLiteral<sizeof...(Ns) + 1>{name[Ns]...};
  };
  return to_str_lit(std::make_index_sequence<name.size()>{});
}

template <class T>
auto get_field_names();

template <class T, auto ptr>
auto get_field_name() {
#if defined(__clang__)
  using Type = std::remove_cvref_t<std::remove_pointer_t<
      typename std::remove_pointer_t<decltype(ptr)>::Type>>;
#else
  using Type = std::remove_cvref_t<std::remove_pointer_t<decltype(ptr)>>;
#endif
  if constexpr (is_rename_v<Type>) {
    using Name = typename Type::Name;
    return Name();
  } else if constexpr (is_flatten_field_v<Type>) {
    return get_field_names<std::remove_cvref_t<typename Type::Type>>();
  } else {
    return rfl::Literal<get_field_name_str_lit<T, ptr>()>();
  }
}

// We don't want the operator+ to apply to normal literals,
// so we introduce this wrapper.
template <StringLiteral... _names>
struct LiteralWrapper {
  Literal<_names...> literal_;
};

template <StringLiteral... _names>
auto wrap_literal(const Literal<_names...>& _literal) {
  return LiteralWrapper<_names...>{_literal};
}

template <StringLiteral... _names1, StringLiteral... _names2>
auto operator+(const LiteralWrapper<_names1...>&,
               const LiteralWrapper<_names2...>&) {
  return LiteralWrapper<_names1..., _names2...>{
      rfl::Literal<_names1..., _names2...>::template from_value<0>()};
}

template <class Head, class... Tail>
auto concat_literals(const Head& _head, const Tail&... _tail) {
  return (wrap_literal(_head) + ... + wrap_literal(_tail)).literal_;
}

// Special case - the struct does not contain rfl::Flatten.
template <StringLiteral _head, StringLiteral... _tail>
auto concat_literals(const rfl::Literal<_head>&,
                     const rfl::Literal<_tail>&...) {
  return rfl::Literal<_head, _tail...>::template from_value<0>();
}

inline auto concat_literals() { return rfl::Literal<>(); }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wundefined-inline"
#endif

template <class T>
#if __GNUC__
#ifndef __clang__
[[gnu::no_sanitize_undefined]]
#endif
#endif
auto get_field_names() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_pointer_v<Type>) {
    return get_field_names<std::remove_pointer_t<T>>();
  } else {
#if defined(__clang__)
    const auto get = []<std::size_t... _is>(std::index_sequence<_is...>) {
      return concat_literals(
          get_field_name<Type,
                         wrap(get_ith_field_from_fake_object<T, _is>())>()...);
    };
#else
    const auto get = []<std::size_t... _is>(std::index_sequence<_is...>) {
      return concat_literals(
          get_field_name<Type, get_ith_field_from_fake_object<T, _is>()>()...);
    };
#endif
    return get(std::make_index_sequence<num_fields<T>>());
  }
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace rfl::internal

#endif
