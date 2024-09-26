#ifndef RFL_INTERNAL_ISDEFAULTIFMISSING_HPP_
#define RFL_INTERNAL_ISDEFAULTIFMISSING_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../DefaultIfMissing.hpp"

namespace rfl {
namespace internal {

template <class T>
class is_default_if_missing;

template <class T>
class is_default_if_missing : public std::false_type {};

template <>
class is_default_if_missing<DefaultIfMissing> : public std::true_type {};

template <class T>
constexpr bool is_default_if_missing_v =
    is_default_if_missing<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
