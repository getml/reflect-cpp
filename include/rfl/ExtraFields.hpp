#ifndef RFL_EXTRAFIELDS_HPP_
#define RFL_EXTRAFIELDS_HPP_

#include "Object.hpp"

namespace rfl {

/// Used to embed additional fields for which the names cannot be known in advance.
/// ExtraFields extends Object to capture unknown or dynamic fields during deserialization.
/// This is useful when you want to preserve fields that aren't part of your struct definition,
/// or when dealing with schemas that allow arbitrary additional properties.
/// For example, in JSON you might receive extra fields beyond what your struct defines.
/// @tparam T The value type for the extra fields (typically a variant or generic type)
template <class T>
class ExtraFields : public Object<T> {};

}  // namespace rfl

#endif
