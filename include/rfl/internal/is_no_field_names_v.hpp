#ifndef RFL_INTERNAL_ISNOFIELDNAMES_HPP_
#define RFL_INTERNAL_ISNOFIELDNAMES_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../NoFieldNames.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_no_field_names;

template <class T>
class is_no_field_names : public std::false_type {};

template <>
class is_no_field_names<NoFieldNames> : public std::true_type {};

template <class T>
constexpr bool is_no_field_names_v =
    is_no_field_names<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
