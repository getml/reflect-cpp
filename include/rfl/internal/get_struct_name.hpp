#ifndef RFL_INTERNAL_GETSTRUCTNAME_HPP_
#define RFL_INTERNAL_GETSTRUCTNAME_HPP_

#include <iostream>
#include <source_location>
#include <string>

namespace rfl {
namespace internal {

inline std::string remove_namespaces(const std::string& _name) {
  const size_t pos = _name.find_last_of(":");
  if (pos == std::string::npos) {
    return _name;
  }
  return _name.substr(pos + 1);
}

inline size_t find_begin(const std::string& _func_name) {
  const size_t pos1 = _func_name.find("T = ");
  if (pos1 != std::string::npos) {
    // clang and gcc
    return pos1 + 4;
  }
  // MSVC
  const size_t pos2 = _func_name.find_last_of(" ");
  return (pos2 != std::string::npos) ? pos2 + 1 : pos2;
}

template <class T>
std::string get_struct_name_impl() {
  const std::string func_name = std::source_location::current().function_name();
  const size_t begin = find_begin(func_name);
  if (begin == std::string::npos) {
    return "AnonymousStruct";
  }
  const size_t end = func_name.find_first_of("])}>;", begin);
  if (end == std::string::npos) {
    return "AnonymousStruct";
  }
  return remove_namespaces(func_name.substr(begin, end - begin));
}

template <class T>
std::string get_struct_name() {
  return get_struct_name_impl<std::decay_t<std::remove_pointer_t<T>>>();
}

}  // namespace internal
}  // namespace rfl

#endif
