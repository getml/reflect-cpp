#ifndef RFL_INTERNAL_IS_NAMED_TUPLE_HPP_
#define RFL_INTERNAL_IS_NAMED_TUPLE_HPP_

#include <type_traits>

#include "../NamedTuple.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_named_tuple;

template <class T>
class is_named_tuple : public std::false_type {};

template <class... Fields>
class is_named_tuple<NamedTuple<Fields...>> : public std::true_type {};

template <class T>
constexpr bool is_named_tuple_v =
    is_named_tuple<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
