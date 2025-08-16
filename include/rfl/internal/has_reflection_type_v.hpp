#ifndef RFL_HASREFLECTIONTYPEV_HPP_
#define RFL_HASREFLECTIONTYPEV_HPP_

#include <concepts>

namespace rfl::internal {

template <class T>
struct ReflectionTypeWrapper {};

template <typename T>
constexpr bool has_reflection_type_v = requires() {
  {
    ReflectionTypeWrapper<typename T::ReflectionType>{}
  } -> std::same_as<ReflectionTypeWrapper<typename T::ReflectionType>>;
};

}  // namespace rfl::internal

#endif  // RFL_HASNAMEDTUPLETYPEV_HPP_
