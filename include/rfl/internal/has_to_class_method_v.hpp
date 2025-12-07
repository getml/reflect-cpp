#ifndef RFL_INTERNAL_HASTOCLASSMETHODV_HPP_
#define RFL_INTERNAL_HASTOCLASSMETHODV_HPP_

#include <concepts>

namespace rfl::internal {

template <class T>
constexpr bool has_to_class_method_v = requires(T t) {
  { t.to_class() };
};

}  // namespace rfl::internal

#endif
