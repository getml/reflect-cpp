#ifndef RFL_HASTAGV_HPP_
#define RFL_HASTAGV_HPP_

#include <cstdint>
#include <utility>

namespace rfl {
namespace internal {

template <class Wrapper>
class HasTag {
 private:
  template <class U>
  static std::int64_t foo(...);

  template <class U>
  static std::int32_t foo(typename U::Tag*);

 public:
  static constexpr bool value =
      sizeof(foo<Wrapper>(nullptr)) == sizeof(std::int32_t);
};

/// Used for tagged unions - determines whether a struct as a Tag.
template <typename Wrapper>
constexpr bool has_tag_v = HasTag<Wrapper>::value;

}  // namespace internal
}  // namespace rfl

#endif
