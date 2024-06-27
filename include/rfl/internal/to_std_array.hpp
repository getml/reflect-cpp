#ifndef RFL_INTERNAL_TO_STD_ARRAY_HPP_
#define RFL_INTERNAL_TO_STD_ARRAY_HPP_

#include <array>
#include <cstdint>
#include <type_traits>

namespace rfl::internal {

template <class T>
struct StdArrayType {
  using Type = T;
};

template <class T, size_t _n>
struct StdArrayType<T[_n]> {
  using Type =
      std::array<typename StdArrayType<std::remove_cvref_t<T>>::Type, _n>;
  using ValueType = std::remove_cvref_t<T>;
  constexpr static size_t size = _n;
};

template <class T>
using to_std_array_t = typename StdArrayType<T>::Type;

template <class T>
auto to_std_array(T&& _t) {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_array_v<Type>) {
    constexpr size_t n = StdArrayType<Type>::size;
    const auto fct = [&]<std::size_t... _i>(std::index_sequence<_i...>) {
      return to_std_array_t<Type>({to_std_array(
          std::forward<typename StdArrayType<Type>::ValueType>(_t[_i]))...});
    };
    return fct(std::make_index_sequence<n>());
  } else {
    return std::forward<T>(_t);
  }
}

template <class T>
auto to_std_array(const T& _t) {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_array_v<Type>) {
    constexpr size_t n = StdArrayType<Type>::size;
    const auto fct = [&]<std::size_t... _i>(std::index_sequence<_i...>) {
      return to_std_array_t<Type>({to_std_array(_t[_i])...});
    };
    return fct(std::make_index_sequence<n>());
  } else {
    return _t;
  }
}

}  // namespace rfl::internal

#endif
