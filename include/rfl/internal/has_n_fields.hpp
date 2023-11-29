#ifndef RFL_INTERNAL_HAS_N_FIELDS_HPP_
#define RFL_INTERNAL_HAS_N_FIELDS_HPP_

#include <cstddef>
#include <type_traits>

namespace rfl {
namespace internal {

struct any {
  template <typename T>
  constexpr operator T() const noexcept;
};

template <typename T, typename Is, typename = void>
struct constructible_with_n_fields_impl : std::false_type {};

template <typename T, std::size_t... Is>
struct constructible_with_n_fields_impl<
    T, std::index_sequence<Is...>,
    std::void_t<decltype(T{(void(Is), any{})...})>> : std::true_type {};

template <typename T, std::size_t N>
using constructible_with_n_fields =
    constructible_with_n_fields_impl<T, std::make_index_sequence<N>>;

template <class T, int _n>
constexpr bool has_n_fields = constructible_with_n_fields<T, _n>::value &&
                              !constructible_with_n_fields<T, _n + 1>::value;

}  // namespace internal
}  // namespace rfl

#endif
