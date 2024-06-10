#ifndef RFL_INTERNAL_ISSTRIPFIELDNAMES_HPP_
#define RFL_INTERNAL_ISSTRIPFIELDNAMES_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../StripFieldNames.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_strip_field_names;

template <class T>
class is_strip_field_names : public std::false_type {};

template <>
class is_strip_field_names<StripFieldNames> : public std::true_type {};

template <class T>
constexpr bool is_strip_field_names_v =
    is_strip_field_names<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
