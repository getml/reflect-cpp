#ifndef RFL_INTERNAL_HASREFLECTIONMETHODV_HPP_
#define RFL_INTERNAL_HASREFLECTIONMETHODV_HPP_

#include <type_traits>

namespace rfl {
namespace internal {

template <typename Wrapper>
using reflection_method_t =
    decltype(std::declval<const Wrapper>().reflection());

template <typename Wrapper, typename = std::void_t<>>
struct has_refl_m : std::false_type {};

template <typename Wrapper>
struct has_refl_m<Wrapper, std::void_t<reflection_method_t<Wrapper>>>
    : std::true_type {};

/// Utility parameter for named tuple parsing, can be used by the
/// parsers to determine whether a class or struct has a method
/// called "reflection".
template <typename Wrapper>
constexpr bool has_reflection_method_v = has_refl_m<Wrapper>::value;

}  // namespace internal
}  // namespace rfl

#endif
