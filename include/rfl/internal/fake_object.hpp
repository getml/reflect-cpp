#ifndef RFL_INTERNAL_FAKEOBJECT_HPP_
#define RFL_INTERNAL_FAKEOBJECT_HPP_

#include "declared_inside_function_or_unnamed_namespace.hpp"

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
  static_assert(
      !declared_inside_function_or_unnamed_namespace<T>(),
      "You should only retrieve field names from structs "
      "that can be externally linked. "
      "This means they cannot be declared inside a function or inside "
      "an unnamed namespace (normal namespaces are fine). This is not "
      "guaranteed "
      "to work on all compiler versions (even though it might work on this "
      "one). To suppress this static assertion and try your luck, pass "
      "REFLECT_CPP_IGNORE_INTERNAL_LINKAGE to the compiler.");
  return fake_object<T>.value;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace internal
}  // namespace rfl

#endif
