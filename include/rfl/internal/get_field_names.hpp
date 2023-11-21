#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

#include <array>

#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/get_field_name.hpp"

namespace rfl {
namespace internal {

template <class MyStruct>
auto get_field_names() {
  auto& [f1, f2] = fake_object<MyStruct>;
  return std::array<std::string, 2>(
      {get_field_name<&f1>(), get_field_name<&f2>()});
}

}  // namespace internal
}  // namespace rfl

#endif
