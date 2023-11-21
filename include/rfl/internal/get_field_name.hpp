#ifndef RFL_INTERNAL_GETFIELDNAME_HPP_
#define RFL_INTERNAL_GETFIELDNAME_HPP_

#include <memory>
#include <source_location>
#include <string>
#include <type_traits>

#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/is_field.hpp"

namespace rfl {
namespace internal {

template <auto* ptr>
std::string get_field_name() {
  using Type = std::decay_t<std::remove_pointer_t<decltype(ptr)>>;
  if constexpr (rfl::internal::is_field_v<Type>) {
    using Name = typename Type::Name;
    return Name().str();
  } else {
    const std::string func_name =
        std::source_location::current().function_name();
    const size_t pos_fake_object = func_name.find("fake_object");
    const size_t begin = func_name.find_first_of("::", pos_fake_object) + 2;
    const size_t end = func_name.find_first_of(")", begin);
    return func_name.substr(begin, end - begin);
  }
}

}  // namespace internal
}  // namespace rfl

#endif
