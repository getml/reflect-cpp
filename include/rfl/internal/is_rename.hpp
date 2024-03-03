#ifndef RFL_INTERNAL_ISRENAME_HPP_
#define RFL_INTERNAL_ISRENAME_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Rename.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_rename;

template <class T>
class is_rename : public std::false_type {};

template <StringLiteral _name, class Type>
class is_rename<Rename<_name, Type>> : public std::true_type {};

template <class T>
constexpr bool is_rename_v =
    is_rename<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
