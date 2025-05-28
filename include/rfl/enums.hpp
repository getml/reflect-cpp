#ifndef RFL_ENUMS_HPP_
#define RFL_ENUMS_HPP_

#include <string>

#include "Result.hpp"
#include "internal/enums/StringConverter.hpp"
#include "internal/enums/get_enum_names.hpp"
#include "internal/enums/is_flag_enum.hpp"
#include "thirdparty/enchantum.hpp"

namespace rfl {

// Converts an enum value to a string.
template <enchantum::Enum EnumType>
std::string enum_to_string(EnumType _enum) {
  return rfl::internal::enums::StringConverter<EnumType>::enum_to_string(_enum);
}

// Converts a string to a value of the given enum type.
template <enchantum::Enum EnumType>
rfl::Result<EnumType> string_to_enum(const std::string& _str) {
  return rfl::internal::enums::StringConverter<EnumType>::string_to_enum(_str);
}

// Returns a named tuple mapping names of enumerators of the given enum type to
// their values.
template <enchantum::Enum EnumType>
auto get_enumerators() {
  constexpr auto names = internal::enums::get_enum_names<
      EnumType, internal::enums::is_flag_enum<EnumType>>();
  return internal::enums::names_to_enumerator_named_tuple(names);
}

// Returns a named tuple mapping names of enumerators of the given enum type to
// their underlying values.
template <enchantum::Enum EnumType>
auto get_underlying_enumerators() {
  constexpr auto names = internal::enums::get_enum_names<
      EnumType, internal::enums::is_flag_enum<EnumType>>();
  return internal::enums::names_to_underlying_enumerator_named_tuple(names);
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and values.
template <enchantum::Enum EnumType>
constexpr auto get_enumerator_array() {
  constexpr auto names = internal::enums::get_enum_names<
      EnumType, internal::enums::is_flag_enum<EnumType>>();
  return internal::enums::names_to_enumerator_array(names);
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and underlying values.
template <enchantum::Enum EnumType>
constexpr auto get_underlying_enumerator_array() {
  constexpr auto names = internal::enums::get_enum_names<
      EnumType, internal::enums::is_flag_enum<EnumType>>();
  return internal::enums::names_to_underlying_enumerator_array(names);
}

// Returns the range of the given enum type as a pair of the minimum and maximum
template <enchantum::Enum EnumType>
constexpr auto get_enum_range() {
  return std::make_pair(
      internal::enums::get_range_min<EnumType,
                                     internal::enums::is_flag_enum<EnumType>>(),
      internal::enums::get_range_max<
          EnumType, internal::enums::is_flag_enum<EnumType>>());
}

}  // namespace rfl

#endif  // RFL_ENUMS_HPP_
