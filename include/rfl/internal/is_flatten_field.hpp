#ifndef RFL_INTERNAL_ISFLATTENFIELD_HPP_
#define RFL_INTERNAL_ISFLATTENFIELD_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Flatten.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_flatten_field;

template <class T>
class is_flatten_field : public std::false_type {};

template <class T>
class is_flatten_field<Flatten<T>> : public std::true_type {};

template <class T>
constexpr bool is_flatten_field_v =
    is_flatten_field<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
