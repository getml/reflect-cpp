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

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wundefined-inline"
#endif

namespace rfl {
namespace internal {

template <class Derived>
struct any_base {
  any_base(std::size_t);
  template <class Base>
    requires(
        std::is_base_of_v<std::remove_cvref_t<Base>,
                          std::remove_cv_t<Derived>> &&
        !std::is_same_v<std::remove_cvref_t<Base>, std::remove_cv_t<Derived>>)
  constexpr operator Base&() const noexcept;
};

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

  template <std::size_t size = 0>
  static consteval std::size_t get_the_sole_nested_base_field_count() {
    static_assert(size <= sizeof(T));
    if constexpr (constructible_with_nested<0, size, 0>() &&
                  !constructible_with_nested<0, size + 1, 0>()) {
      return size;
    } else {
      return get_the_sole_nested_base_field_count<size + 1>();
    }
  }

  template <std::size_t n, std::size_t total_arg_num>
  static consteval bool has_n_base_param() {
    constexpr auto right_len = total_arg_num>=n ? total_arg_num-n : 0;
    return []<std::size_t... l, std::size_t... r>(std::index_sequence<l...>,
                                                  std::index_sequence<r...>) {
      return requires { T{any_base<T>(l)..., any(r)...}; };
    }(std::make_index_sequence<n>(), std::make_index_sequence<right_len>());
  }

  template <std::size_t total_arg_num, std::size_t index = 0>
  static consteval std::size_t base_param_num() {
    if constexpr (!has_n_base_param<index + 1, total_arg_num>()) {
      return index;
    } else {
      return base_param_num<total_arg_num, index + 1>();
    }
  }

  template <std::size_t index, std::size_t max>
  static consteval std::size_t constructible_no_brace_elision() {
    static_assert(index <= max);
    if constexpr (index == max) {
      return 0;
    } else {
      return 1 +
             constructible_no_brace_elision<
                 index + get_nested_array_size<index, max - index, 0>(), max>();
    }
  }

  static consteval std::size_t count_fields() {
    constexpr std::size_t max_fields = count_max_fields();
    constexpr std::size_t total_args =
        constructible_no_brace_elision<0, max_fields>();
    constexpr std::size_t base_args = base_param_num<total_args>();
    if constexpr (total_args == 0 && base_args == 0) {
      // Empty struct
      return 0;
    } else if constexpr (base_args == total_args) {
      // Special case when the derived class is empty.
      // In such cases the filed number is the fields in base class.
      // Note that there should be only one base class in this case.
      return get_the_sole_nested_base_field_count();
    } else {
      return total_args - base_args;
    }
  }
};

template <class T>
constexpr std::size_t num_fields = CountFieldsHelper<T>::count_fields();

}  // namespace internal
}  // namespace rfl

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
