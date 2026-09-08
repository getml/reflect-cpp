#ifndef RFL_SKIP_HPP_
#define RFL_SKIP_HPP_

#include "internal/Skip.hpp"

namespace rfl {

/// Type alias for skipping a field in both serialization and deserialization.
/// When a struct field is wrapped in Skip<T>, it will be completely ignored during
/// reading and writing of serialized formats.
/// @tparam T The type of the field to skip
template <class T>
using Skip = internal::Skip<T, true, true>;

/// Type alias for skipping a field only during serialization (writing).
/// The field will be read from serialized data but not written to it.
/// @tparam T The type of the field to skip during serialization
template <class T>
using SkipSerialization = internal::Skip<T, true, false>;

/// Type alias for skipping a field only during deserialization (reading).
/// The field will be written to serialized data but not read from it.
/// @tparam T The type of the field to skip during deserialization
template <class T>
using SkipDeserialization = internal::Skip<T, false, true>;

}  // namespace rfl

#endif

