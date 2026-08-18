#ifndef RFL_NUM_FIELDS_HPP_
#define RFL_NUM_FIELDS_HPP_

#ifndef REFLECTCPP_USE_CPP26_REFLECTION
#include "internal/cpp20/num_fields.hpp"
#else
#include "internal/cpp26/num_fields.hpp"
#endif

namespace rfl {

/// Returns the number of fields in struct T at compile time.
/// @tparam T The struct type to count fields for
#ifndef REFLECTCPP_USE_CPP26_REFLECTION
template <class T>
constexpr std::size_t num_fields = internal::cpp20::num_fields<T>;
#else
template <class T>
constexpr std::size_t num_fields = internal::cpp26::num_fields<T>;
#endif

}  // namespace rfl

#endif
