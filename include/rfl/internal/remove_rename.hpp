#ifndef RFL_INTERNAL_REMOVERENAME_HPP_
#define RFL_INTERNAL_REMOVERENAME_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../Rename.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <class T>
struct remove_rename;

template <class T>
struct remove_rename {
  using Type = T;
};

template <StringLiteral _name, class _Type>
struct remove_rename<Rename<_name, _Type>> {
  using Type = _Type;
};

template <class T>
using remove_rename_t =
    typename remove_rename<std::remove_cvref_t<std::remove_pointer_t<T>>>::Type;

}  // namespace internal
}  // namespace rfl

#endif
