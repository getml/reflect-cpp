#ifndef RFL_ENUMS_HPP_
#define RFL_ENUMS_HPP_

#include <sstream>
#include <string>

#include "Result.hpp"
#include "internal/enums/from_string.hpp"
#include "internal/enums/get_enum_names.hpp"
#include "internal/enums/get_enum_range.hpp"
#include "internal/enums/is_flag_enum.hpp"
#include "internal/enums/to_string.hpp"
#include "internal/strings/strings.hpp"

namespace rfl {

// Returns a named tuple mapping names of enumerators of the given enum type to
// their values.
template <class EnumType>
auto get_enumerators() {
  return internal::enums::names_to_enumerator_named_tuple(
      internal::enums::get_enum_names<EnumType>());
}

// Returns a named tuple mapping names of enumerators of the given enum type to
// their underlying values.
template <class EnumType>
auto get_underlying_enumerators() {
  return internal::enums::names_to_underlying_enumerator_named_tuple(
      internal::enums::get_enum_names<EnumType>());
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and values.
template <class EnumType>
constexpr auto get_enumerator_array() {
  return internal::enums::names_to_enumerator_array(
      internal::enums::get_enum_names<EnumType>());
}

// Returns an std::array containing pairs of enumerator names (as
// std::string_view) and underlying values.
template <class EnumType>
constexpr auto get_underlying_enumerator_array() {
  return internal::enums::names_to_underlying_enumerator_array(
      internal::enums::get_enum_names<EnumType>());
}

// Returns the range of the given enum type as a pair of the minimum and maximum
template <class EnumType>
constexpr auto get_enum_range() {
  return internal::enums::get_enum_range<EnumType>();
}

// Converts an enum value to tis string representation.
template <class EnumType>
std::string enum_to_string(const EnumType _enum) {
  if constexpr (internal::enums::is_flag_enum<EnumType>) {
    // Iterates through the enum bit by bit and matches it against the flags.
    using T = std::underlying_type_t<EnumType>;
    auto val = static_cast<T>(_enum);
    int i = 0;
    std::vector<std::string> flags;
    while (val != 0) {
      const auto bit = val & static_cast<T>(1);
      if (bit == 1) {
        auto str = internal::enums::to_string(
            static_cast<EnumType>(static_cast<T>(1) << i));
        flags.emplace_back(std::move(str));
      }
      ++i;
      val >>= 1;
    }
    if (flags.empty()) {
      return "0";
    }
    return internal::strings::join("|", flags);
  } else {
    return internal::enums::to_string(_enum);
  }
}

// Converts a string to a value of the given enum type.
template <class EnumType>
Result<EnumType> string_to_enum(const std::string& _str) {
  const auto cast_numbers_or_names =
      [](const std::string& name) -> Result<EnumType> {
    const auto r = internal::enums::from_string<EnumType>(name);
    if (r) {
      return *r;
    }
    try {
      return static_cast<EnumType>(std::stoi(name));
    } catch (std::exception& exp) {
      std::string msg = "Invalid enum value: '";
      msg += name;
      msg += "'. Must be one of [";
      const char* sep = "";
      for (const auto& p : get_enumerator_array<EnumType>()) {
        msg += sep;
        msg += p.first;
        sep = ", ";
      }
      msg += "].";
      return error(msg);
    }
  };

  if constexpr (internal::enums::is_flag_enum<EnumType>) {
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

}  // namespace rfl

#endif  // RFL_ENUMS_HPP_
