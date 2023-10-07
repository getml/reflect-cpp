#ifndef RFL_INTERNAL_HASTOCLASSMETHODV_HPP_
#define RFL_INTERNAL_HASTOCLASSMETHODV_HPP_

#include <type_traits>

namespace rfl {
namespace internal {

template <typename Wrapper>
using to_class_method_t = decltype(std::declval<const Wrapper>().to_class());

template <typename Wrapper, typename = std::void_t<>>
struct has_to_class_m : std::false_type {};

template <typename Wrapper>
struct has_to_class_m<Wrapper, std::void_t<to_class_method_t<Wrapper>>>
    : std::true_type {};

/// Utility parameter for named tuple parsing, can be used by the
/// parsers to determine whether a class or struct has a method
/// called "to_class".
template <typename Wrapper>
constexpr bool has_to_class_method_v = has_to_class_m<Wrapper>::value;

}  // namespace internal
}  // namespace rfl

#endif
