#ifndef RFL_INTERNAL_ISNOEXTRAFIELDS_HPP_
#define RFL_INTERNAL_ISNOEXTRAFIELDS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../NoExtraFields.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_no_extra_fields;

template <class T>
class is_no_extra_fields : public std::false_type {};

template <>
class is_no_extra_fields<NoExtraFields> : public std::true_type {};

template <class T>
constexpr bool is_no_extra_fields_v =
    is_no_extra_fields<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
