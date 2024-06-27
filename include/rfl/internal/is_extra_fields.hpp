#ifndef RFL_INTERNAL_ISEXTRAFIELDS_HPP_
#define RFL_INTERNAL_ISEXTRAFIELDS_HPP_

#include <type_traits>

#include "../ExtraFields.hpp"

namespace rfl::internal {

template <class T>
class is_extra_fields;

template <class T>
class is_extra_fields : public std::false_type {};

template <class T>
class is_extra_fields<ExtraFields<T>> : public std::true_type {};

template <class T>
constexpr bool is_extra_fields_v =
    is_extra_fields<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
