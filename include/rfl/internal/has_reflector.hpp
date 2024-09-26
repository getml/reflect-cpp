#ifndef RFL_INTERNAL_HASREFLECTOR_HPP_
#define RFL_INTERNAL_HASREFLECTOR_HPP_

#include <type_traits>

namespace rfl {
template <typename T>
struct Reflector;

namespace internal {

template <typename Type>
concept has_write_reflector = requires(Type&& item) {
  Reflector<Type>::from(item);
};

template <typename Type>
concept has_read_reflector =
    requires(const typename Reflector<Type>::ReflType& item) {
  Reflector<Type>::to(item);
};

}  // namespace internal
}  // namespace rfl

#endif
