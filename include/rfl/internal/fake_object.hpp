#ifndef RFL_INTERNAL_FAKEOBJECT_HPP_
#define RFL_INTERNAL_FAKEOBJECT_HPP_

namespace rfl {
namespace internal {

/// This is required to automatically retrieve the names.
template <class T>
extern T fake_object;

}  // namespace internal
}  // namespace rfl

#endif
