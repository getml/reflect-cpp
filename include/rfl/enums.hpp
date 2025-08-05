#ifndef RFL_ENUMS_HPP_
#define RFL_ENUMS_HPP_

#include <string>

#include "Result.hpp"
#include "internal/enums/get_enum_names.hpp"
#include "internal/strings/strings.hpp"
#include "thirdparty/enchantum/enchantum.hpp"
#include "thirdparty/enchantum/bitflags.hpp"

namespace rfl {

template <enchantum::Enum EnumType>
std::string enum_to_string(const EnumType _enum) {
  const auto to_string_or_number = [](const EnumType e) {
    const auto s = enchantum::to_string(e);
    return s.empty() ? std::to_string(
                           static_cast<std::underlying_type_t<EnumType>>(e))
                     : std::string(s);
  };

  if constexpr (enchantum::is_bitflag<EnumType>) {
    // Iterates through the enum bit by bit and matches it against the flags.
    using T = std::underlying_type_t<EnumType>;
    auto val = static_cast<T>(_enum);
    int i = 0;
    std::vector<std::string> flags;
    while (val != 0) {
      const auto bit = val & static_cast<T>(1);
      if (bit == 1) {
        auto str =
            to_string_or_number(static_cast<EnumType>(static_cast<T>(1) << i));
        flags.emplace_back(std::move(str));
      }
      ++i;
      val >>= 1;
    }
    return internal::strings::join("|", flags);
  } else {
    return to_string_or_number(_enum);
  }
}

// Converts a string to a value of the given enum type.
template <enchantum::Enum EnumType>
Result<EnumType> string_to_enum(const std::string& _str) {
  const auto cast_numbers_or_names =
      [](const std::string& name) -> Result<EnumType> {
    const auto r = enchantum::cast<EnumType>(name);
    if (r) return *r;
    try {
      return static_cast<EnumType>(std::stoi(name));
    } catch (std::exception& exp) {
      return error(exp.what());
    }
  };

  if constexpr (enchantum::is_bitflag<EnumType>) {
    using T = std::underlying_type_t<EnumType>;
    const auto split = internal::strings::split(_str, "|");
    auto res = static_cast<T>(0);
    for (const auto& s : split) {
      const auto r = cast_numbers_or_names(s);
      if (r) {
        res |= static_cast<T>(*r);
      } else {
        return r;
      }
    }
    return static_cast<EnumType>(res);
  } else {
    return cast_numbers_or_names(_str);
  }
}

// Returns a named tuple mapping names of enumerators of the given enum type to
// their values.
template <enchantum::Enum EnumType>
auto get_enumerators() {
  return internal::enums::names_to_enumerator_named_tuple(
      internal::enums::get_enum_names<EnumType>());
}

// Returns a named tuple mapping names of enumerators of the given enum type to
// their underlying values.
template <enchantum::Enum EnumType>
auto get_underlying_enumerators() {
  return internal::enums::names_to_underlying_enumerator_named_tuple(
      internal::enums::get_enum_names<EnumType>());
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and values.
template <enchantum::Enum EnumType>
constexpr auto get_enumerator_array() {
  return internal::enums::names_to_enumerator_array(
      internal::enums::get_enum_names<EnumType>());
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and underlying values.
template <enchantum::Enum EnumType>
constexpr auto get_underlying_enumerator_array() {
  return internal::enums::names_to_underlying_enumerator_array(
      internal::enums::get_enum_names<EnumType>());
}

// Returns the range of the given enum type as a pair of the minimum and maximum
template <enchantum::Enum EnumType>
constexpr auto get_enum_range() {
  return std::make_pair(enchantum::enum_traits<EnumType>::min,
                        enchantum::enum_traits<EnumType>::max);
}

}  // namespace rfl

#endif  // RFL_ENUMS_HPP_
