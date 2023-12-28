#ifndef RFL_FIELDS_HPP_
#define RFL_FIELDS_HPP_

#include "internal/get_meta_fields.hpp"
#include "named_tuple_t.hpp"

namespace rfl {

/// Returns meta-information about the fields.
template <class T>
auto fields() {
  return internal::get_meta_fields<named_tuple_t<T>>();
}

}  // namespace rfl

#endif
