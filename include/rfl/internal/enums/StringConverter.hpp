#ifndef RFL_INTERNAL_ENUMS_STRINGCONVERTER_HPP_
#define RFL_INTERNAL_ENUMS_STRINGCONVERTER_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>

#include "../../Result.hpp"
#include "../../internal/strings/strings.hpp"
#include "../../type_name_t.hpp"
#include "get_enum_names.hpp"
#include "is_flag_enum.hpp"

namespace rfl ::internal ::enums {

template <class EnumType>
class StringConverter {
 public:
  static constexpr bool is_flag_enum_ = is_flag_enum<EnumType>;

  static constexpr auto names_ = get_enum_names<EnumType, is_flag_enum_>();

  using NamesLiteral = typename decltype(names_)::Literal;

 public:
  /// Transform an enum to a matching string.
  static std::string enum_to_string(const EnumType _enum) {
    if constexpr (is_flag_enum_) {
      return flag_enum_to_string(_enum);
    } else {
      return enum_to_single_string(_enum);
    }
  }

  /// Transforms a string to the matching enum.
  static Result<EnumType> string_to_enum(const std::string& _str) {
    static_assert(names_.size != 0,
                  "No enum could be identified. Please choose enum values "
                  "between 0 to 127 or for flag enums choose 1,2,4,8,16,...");
    if constexpr (is_flag_enum_) {
      return string_to_flag_enum(_str);
    } else {
      return single_string_to_enum(_str);
    }
  }

 private:
  /// Iterates through the enum bit by bit and matches it against the flags.
  static std::string flag_enum_to_string(const EnumType _e) {
    using T = std::underlying_type_t<EnumType>;
    auto val = static_cast<T>(_e);
    int i = 0;
    std::vector<std::string> flags;
    while (val != 0) {
      const auto bit = val & static_cast<T>(1);
      if (bit == 1) {
        auto str = enum_to_single_string(
            static_cast<EnumType>(static_cast<T>(1) << i));
        flags.emplace_back(std::move(str));
      }
      ++i;
      val >>= 1;
    }
    return strings::join("|", flags);
  }

  /// This assumes that _enum can be exactly matched to one of the names and
  /// does not have to be combined using |.
  static std::string enum_to_single_string(const EnumType _enum) {
    const auto to_str = [](const auto _l) { return _l.str(); };

    for (size_t i = 0; i < names_.size; ++i) {
      if (names_.enums_[i] == _enum) {
        return NamesLiteral::from_value(
                   static_cast<typename NamesLiteral::ValueType>(i))
            .transform(to_str)
            .value();
      }
    }

    return std::to_string(static_cast<std::underlying_type_t<EnumType>>(_enum));
  }

  /// Finds the enum matching the literal.
  static EnumType literal_to_enum(const NamesLiteral _lit) noexcept {
    return names_.enums_[_lit.value()];
  }

  /// This assumes that _enum can be exactly matched to one of the names and
  /// does not have to be combined using |.
  static Result<EnumType> single_string_to_enum(const std::string& _str) {
    const auto r = NamesLiteral::from_string(_str).transform(literal_to_enum);
    if (r) {
      return r;
    } else {
      try {
        return static_cast<EnumType>(std::stoi(_str));
      } catch (std::exception& exp) {
        return error(exp.what());
      }
    }
  }

  /// Only relevant if this is a flag enum - combines the different matches
  /// using |.
  static Result<EnumType> string_to_flag_enum(
      const std::string& _str) noexcept {
    using T = std::underlying_type_t<EnumType>;
    const auto split = strings::split(_str, "|");
    auto res = static_cast<T>(0);
    for (const auto& s : split) {
      const auto r = single_string_to_enum(s);
      if (r) {
        res |= static_cast<T>(*r);
      } else {
        return r;
      }
    }
    return static_cast<EnumType>(res);
  }
};

}  // namespace rfl::internal::enums

#endif
