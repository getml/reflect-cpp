#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

#include <array>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <type_traits>
#include <utility>

#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/is_rename.hpp"
#include "rfl/internal/num_fields.hpp"

namespace rfl {
namespace internal {

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

template <class T>
auto get_field_names();

template <int i, auto ptr>
auto get_field_name() {
  using Type = std::decay_t<std::remove_pointer_t<
      typename std::remove_pointer_t<decltype(ptr)>::Type>>;
  if constexpr (is_rename_v<Type>) {
    using Name = typename Type::Name;
    return std::array<std::string, 1>{Name().str()};
  } else if constexpr (is_flatten_field_v<Type>) {
    return get_field_names<std::decay_t<typename Type::Type>>();
  } else {
    const std::string func_name =
        std::source_location::current().function_name();
    const size_t pos = func_name.find(">->");
    const size_t begin =
        (pos != std::string::npos) ? pos + 3 : func_name.find_last_of(":.") + 1;
    if (begin == std::string::npos) {
      return std::array<std::string, 1>{"f_" + std::to_string(i)};
    }
    const size_t end = func_name.find_first_of("])}>", begin);
    if (end == std::string::npos) {
      return std::array<std::string, 1>{"f_" + std::to_string(i)};
    }
    return std::array<std::string, 1>{func_name.substr(begin, end - begin)};
  }
}

template <auto N1, auto N2>
auto concat_two_arrays(std::array<std::string, N1>&& _arr1,
                       std::array<std::string, N2>&& _arr2) {
  std::array<std::string, N1 + N2> res;
  for (size_t i = 0; i < N1; ++i) {
    res[i] = std::move(_arr1[i]);
  }
  for (size_t i = 0; i < N2; ++i) {
    res[i + N1] = std::move(_arr2[i]);
  }
  return res;
}

template <class Head, class... Tail>
auto concat_arrays(Head&& _head, Tail&&... _tail) {
  if constexpr (sizeof...(_tail) == 0) {
    return std::move(_head);
  } else {
    return concat_two_arrays(std::move(_head),
                             concat_arrays(std::move(_tail)...));
  }
}

template <class T>
#if __GNUC__
[[gnu::no_sanitize_undefined]]
#endif
auto get_field_names() {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return get_field_names<std::remove_pointer_t<T>>();
  } else
    return []<std::size_t... Is>(std::index_sequence<Is...>) {
      return concat_arrays(
          get_field_name<Is + 1, wrap(std::get<Is>(
                                     to_ptr_tuple(fake_object<T>)))>()...);
    }(std::make_index_sequence<num_fields<T>>());
}

}  // namespace internal
}  // namespace rfl

#endif
