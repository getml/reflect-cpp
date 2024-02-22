#ifndef RFL_ENUMS_HPP_
#define RFL_ENUMS_HPP_

#include <string>

#include "Result.hpp"
#include "internal/enums/StringConverter.hpp"
#include "internal/enums/is_scoped_enum.hpp"

namespace rfl {

template <internal::enums::is_scoped_enum EnumType>
std::string enum_to_string(EnumType _enum) {
  return rfl::internal::enums::StringConverter<EnumType>::enum_to_string(_enum);
}

template <internal::enums::is_scoped_enum EnumType>
rfl::Result<EnumType> string_to_enum(const std::string& _str) {
  return rfl::internal::enums::StringConverter<EnumType>::string_to_enum(_str);
}

}  // namespace rfl

#endif  // RFL_ENUMS_HPP_
