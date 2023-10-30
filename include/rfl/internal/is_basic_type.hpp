#ifndef RFL_INTERNAL_ISBASICTYPE_HPP_
#define RFL_INTERNAL_ISBASICTYPE_HPP_

#include <string>
#include <type_traits>

namespace rfl {
namespace internal {

template <class T>
constexpr bool is_basic_type_v = std::is_floating_point_v<std::decay_t<T>> ||
                                 std::is_integral_v<std::decay_t<T>> ||
                                 std::is_same<std::decay_t<T>, std::string>() ||
                                 std::is_same<std::decay_t<T>, bool>();

}  // namespace internal
}  // namespace rfl

#endif
