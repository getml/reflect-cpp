#ifndef RFL_NUM_FIELDS_HPP_
#define RFL_NUM_FIELDS_HPP_

#include "internal/num_fields.hpp"

namespace rfl {

/// Returns the number of fields fields.
template <class T>
constexpr std::size_t num_fields = internal::num_fields<T>;

}  // namespace rfl

#endif
