#ifndef RFL_HASTAGV_HPP_
#define RFL_HASTAGV_HPP_

#include <concepts>

namespace rfl::internal {

template <class T>
struct TagWrapper {};

/// Used for tagged unions - determines whether a struct as a Tag.
template <typename T>
constexpr bool has_tag_v = requires() {
  {
    TagWrapper<typename T::Tag>{}
  } -> std::same_as<TagWrapper<typename T::Tag>>;
};

}  // namespace rfl::internal

#endif
