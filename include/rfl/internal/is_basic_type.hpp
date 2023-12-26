#ifndef RFL_INTERNAL_ISBASICTYPE_HPP_
#define RFL_INTERNAL_ISBASICTYPE_HPP_

#include <string>
#include <type_traits>

namespace rfl {
namespace internal {

template <class T>
constexpr bool is_basic_type_v =
    std::is_floating_point_v<std::remove_cvref_t<T>> ||
    std::is_integral_v<std::remove_cvref_t<T>> ||
    std::is_same<std::remove_cvref_t<T>, std::string>() ||
    std::is_same<std::remove_cvref_t<T>, bool>();

}  // namespace internal
}  // namespace rfl

#endif
