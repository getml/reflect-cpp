#ifndef RFL_INTERNAL_NUM_FIELDS_HPP_
#define RFL_INTERNAL_NUM_FIELDS_HPP_

#include <cstddef>

#include "rfl/internal/has_n_fields.hpp"

namespace rfl {
namespace internal {

template <class T, std::size_t _n>
struct num_fields_impl {
  static constexpr std::size_t value = num_fields_impl<T, _n + 1>::value;
};

template <class T, std::size_t _n>
  requires has_n_fields<T, _n>
struct num_fields_impl<T, _n> {
  static constexpr std::size_t value = _n;
};

template <class T>
constexpr std::size_t num_fields = num_fields_impl<T, 0>::value;

}  // namespace internal
}  // namespace rfl

#endif
