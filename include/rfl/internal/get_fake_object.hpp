#ifndef RFL_INTERNAL_GETFAKEOBJECT_HPP_
#define RFL_INTERNAL_GETFAKEOBJECT_HPP_

namespace rfl {
namespace internal {

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wundefined-internal"
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 7631)
#endif

template <class T>
struct wrapper {
  const T value;
  static const wrapper<T> report_if_you_see_a_link_error_with_this_object;
};

template <class T>
consteval const T& get_fake_object() noexcept {
  return wrapper<T>::report_if_you_see_a_link_error_with_this_object.value;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace internal
}  // namespace rfl

#endif
