#ifndef RFL_NUM_FIELDS_HPP_
#define RFL_NUM_FIELDS_HPP_

#include "internal/num_fields.hpp"

namespace rfl {

/// Returns the number of fields in struct T at compile time.
/// This uses structured bindings to count the fields.
/// @tparam T The struct type to count fields for
template <class T>
constexpr std::size_t num_fields = internal::num_fields<T>;

}  // namespace rfl

#endif
