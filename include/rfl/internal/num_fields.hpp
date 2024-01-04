#ifndef RFL_INTERNAL_NUM_FIELDS_HPP_
#define RFL_INTERNAL_NUM_FIELDS_HPP_

/*
We infer the number of fields using by figuring out how many fields
we need to construct it. This is done by implementing the constructible
concept, see below.

However, there is a problem with C arrays. Suppose you have a struct
like this:

struct A{
    int arr[3];
};

Then, the struct can be initialized like this:

const auto a = A{1, 2, 3};

This is a problem, because a naive logic would believe that A
has three fields, when in fact it has only one.

That is why we use the constructible concept to get the maximum
possible number of fields and then try to subdivide them into arrays
in order to figure out which of these fields is in fact an array.

Basically, for every field there is, we try to squeeze as many variables into
the potential array as we can without missing variables in subsequent fields.
This is the purpose of get_nested_array_size().
*/

#include <cstddef>
#include <type_traits>
#include <utility>

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

namespace rfl {
namespace internal {

struct any {
  any(std::size_t);
  template <typename T>
  constexpr operator T() const noexcept;
};

template <typename T>
struct CountFieldsHelper {
  template <std::size_t n>
  static consteval bool constructible() {
    return []<std::size_t... is>(std::index_sequence<is...>) {
      return requires { T{any(is)...}; };
    }
    (std::make_index_sequence<n>());
  }

  template <std::size_t l, std::size_t nested, std::size_t r>
  static consteval bool constructible_with_nested() {
    return []<std::size_t... i, std::size_t... j, std::size_t... k>(
        std::index_sequence<i...>, std::index_sequence<j...>,
        std::index_sequence<k...>) {
      return requires { T{any(i)..., {any(j)...}, any(k)...}; };
    }
    (std::make_index_sequence<l>(), std::make_index_sequence<nested>(),
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
constexpr std::size_t num_fields = CountFieldsHelper<T>::count_fields();

}  // namespace internal
}  // namespace rfl

#endif
