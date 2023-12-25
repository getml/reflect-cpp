#ifndef RFL_INTERNAL_NUM_FIELDS_HPP_
#define RFL_INTERNAL_NUM_FIELDS_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

namespace rfl {
namespace internal {

struct any {
  any(std::size_t);
  template <typename T>
  constexpr operator T() const noexcept;
};

template <typename T>
struct count_fields_helper {
  template <std::size_t n>
  static consteval bool constructible() {
    return []<std::size_t... is>(std::index_sequence<is...>) {
      return requires { T{any(is)...}; };
    }(std::make_index_sequence<n>());
  }

  template <std::size_t l, std::size_t nested, std::size_t r>
  static consteval bool constructible_with_nested() {
    return []<std::size_t... i, std::size_t... j, std::size_t... k>(
               std::index_sequence<i...>, std::index_sequence<j...>,
               std::index_sequence<k...>) {
      return requires { T{any(i)..., {any(j)...}, any(k)...}; };
    }(std::make_index_sequence<l>(), std::make_index_sequence<nested>(),
           std::make_index_sequence<r>());
  }

  template <std::size_t n = 0>
  static consteval std::size_t count_max_fields() {
    static_assert(n <= static_cast<std::size_t>(sizeof(T)));
    if constexpr (constructible<n>() && !constructible<n + 1>()) {
      return n;
    } else {
      return count_max_fields<n + 1>();
    }
  }

  template <std::size_t index, std::size_t size, std::size_t rest>
  static consteval std::size_t get_nested_array_size() {
    if constexpr (size < 1) {
      return 1;
    } else if constexpr (constructible_with_nested<index, size, rest>() &&
                         !constructible_with_nested<index, size, rest + 1>()) {
      return size;
    } else {
      return get_nested_array_size<index, size - 1, rest + 1>();
    }
  }

  template <std::size_t index, std::size_t max>
  static consteval std::size_t count_fields_impl() {
    static_assert(index <= max);
    if constexpr (index == max) {
      return 0;
    } else {
      return 1 +
             count_fields_impl<
                 index + get_nested_array_size<index, max - index, 0>(), max>();
    }
  }

  static consteval std::size_t count_fields() {
    return count_fields_impl<0, count_max_fields()>();
  }
};

template <class T>
constexpr std::size_t num_fields = count_fields_helper<T>::count_fields();

}  // namespace internal
}  // namespace rfl

#endif
