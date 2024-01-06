#ifndef RFL_INTERNAL_DECLARED_INSIDE_FUNCTION_OR_UNNAMED_NAMESPACE_HPP_
#define RFL_INTERNAL_DECLARED_INSIDE_FUNCTION_OR_UNNAMED_NAMESPACE_HPP_

#include <string_view>

namespace rfl {
namespace internal {

template <class T>
consteval bool declared_inside_function_or_unnamed_namespace() {
#if defined(REFLECT_CPP_IGNORE_INTERNAL_LINKAGE)
  return false;
#else
  constexpr auto type_name = get_type_name_str_view<T>();
  constexpr auto pos = type_name.find_last_of(":");
  const auto namespaces = type_name.substr(0, pos);
#if defined(__clang__)
  const bool inside_function = namespaces.find("(") != namespaces.npos &&
                               namespaces.find(")") != namespaces.npos;
  const bool inside_unnamed_namespace =
      namespaces.find("(anonymous namespace)") != std::string_view::npos;
  return inside_function || inside_unnamed_namespace;
#elif defined(__GNUC__)
  return (namespaces.find("{anonymous}") != namespaces.npos);
#elif defined(_MSC_VER)
  return (namespaces.find("anonymous-namespace") != namespaces.npos);
#else
  return false;
#endif
#endif
}

}  // namespace internal
}  // namespace rfl

#endif
