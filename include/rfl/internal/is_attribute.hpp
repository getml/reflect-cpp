#ifndef RFL_INTERNAL_ISATTRIBUTE_HPP_
#define RFL_INTERNAL_ISATTRIBUTE_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Attribute.hpp"
#include "../Field.hpp"
#include "remove_rename.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_attribute;

template <class T>
class is_attribute : public std::false_type {};

template <class Type>
class is_attribute<Attribute<Type>> : public std::true_type {};

template <class T>
constexpr bool is_attribute_v = is_attribute<remove_rename_t<T>>::value;

}  // namespace internal
}  // namespace rfl

#endif
