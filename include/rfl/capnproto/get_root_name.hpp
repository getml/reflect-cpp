#ifndef RFL_CAPNPROTO_GET_ROOT_NAME_HPP_
#define RFL_CAPNPROTO_GET_ROOT_NAME_HPP_

#include <string>

#include "../internal/has_reflection_type_v.hpp"
#include "../parsing/make_type_name.hpp"

namespace rfl::capnproto {

template <class T>
inline std::string get_root_name() {
  if constexpr (internal::has_reflection_type_v<T>) {
    return get_root_name<std::remove_cvref_t<typename T::ReflectionType>>();
  } else {
    return internal::strings::to_pascal_case(parsing::make_type_name<T>());
  }
}

}  // namespace rfl::capnproto

#endif
