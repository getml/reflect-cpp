#ifndef RFL_INTERNAL_FAKEOBJECT_HPP_
#define RFL_INTERNAL_FAKEOBJECT_HPP_

// This makes use of the fact that exernally linked objects must expose the
// names of their field names. Therefore, we can use this to retrieve the field
// names.

namespace rfl {
namespace internal {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

template <class T>
struct struct_wrapper {
  const T value;
};

template <class T>
extern const struct_wrapper<T> fake_object;

template <class T>
consteval const T& get_fake_object() noexcept {
  return fake_object<T>.value;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace internal
}  // namespace rfl

#endif
