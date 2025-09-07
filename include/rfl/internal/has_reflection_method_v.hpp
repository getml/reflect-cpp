#ifndef RFL_INTERNAL_HASREFLECTIONMETHODV_HPP_
#define RFL_INTERNAL_HASREFLECTIONMETHODV_HPP_

#include <concepts>

namespace rfl::internal {

template <class T>
constexpr bool has_reflection_method_v = requires(T t) {
  { t.reflection() } -> std::convertible_to<typename T::ReflectionType>;
};

}  // namespace rfl::internal

#endif
