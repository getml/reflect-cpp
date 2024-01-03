#ifndef RFL_NAME_T_HPP_
#define RFL_NAME_T_HPP_

#include <type_traits>

namespace rfl {

/// Convenience class to retrieve the name of a field.
template <class FieldType>
using name_t = typename std::remove_cvref_t<FieldType>::Name;

}  // namespace rfl

#endif
