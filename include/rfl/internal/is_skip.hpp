#ifndef RFL_INTERNAL_ISSKIP_HPP_
#define RFL_INTERNAL_ISSKIP_HPP_

#include <type_traits>

#include "Skip.hpp"

namespace rfl::internal {

template <class T>
class is_skip;

template <class T>
class is_skip : public std::false_type {};

template <class T, bool _skip_serialization, bool _skip_deserialization>
class is_skip<Skip<T, _skip_serialization, _skip_deserialization>>
    : public std::true_type {};

template <class T>
constexpr bool is_skip_v =
    is_skip<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
