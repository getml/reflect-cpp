#ifndef RFL_EXTRAFIELDS_HPP_
#define RFL_EXTRAFIELDS_HPP_

#include "Object.hpp"

namespace rfl {

/// Used to embed additional fields for which the names cannot be known in
/// advance and can therefore not be encoded in the struct.
template <class T>
class ExtraFields : public Object<T> {};

}  // namespace rfl

#endif
