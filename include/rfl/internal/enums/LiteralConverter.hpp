#ifndef RFL_INTERNAL_ENUMS_LITERAL_CONVERTER_HPP_
#define RFL_INTERNAL_ENUMS_LITERAL_CONVERTER_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/internal/enums/get_enum_names.hpp"
#include "rfl/type_name_t.hpp"

namespace rfl {
namespace internal {
namespace enums {

template <class EnumType>
struct LiteralConverter {
  static constexpr auto names_ = get_enum_names<EnumType>();

  using NamesLiteral = typename decltype(names_)::Literal;

  static Result<NamesLiteral> enum_to_literal(const EnumType _enum) {
    for (size_t i = 0; i < names_.size; ++i) {
      if (names_.enums_[i] == _enum) {
        return NamesLiteral::from_value(
            static_cast<typename NamesLiteral::ValueType>(i));
      }
    }
    return Error(
        "Something went wrong when parsing the enum type '" +
        type_name_t<EnumType>().str() +
        "'. An integer representation of the enum type is " +
        std::to_string(static_cast<std::underlying_type_t<EnumType>>(_enum)) +
        ". Is it possible that some of the values are negative or "
        "greater than 99? This is unsupported (as is explicitly "
        "stated in the documentation).");
  }

  static Result<std::string> enum_to_string(const EnumType _enum) {
    const auto to_str = [](const auto _l) { return _l.str(); };
    return enum_to_literal(_enum).transform(to_str);
  }

  static EnumType literal_to_enum(const NamesLiteral _lit) {
    static_assert(names_.size != 0, "This does not work for empty enums.");
    return names_.enums_[_lit.value()];
  }

  static Result<EnumType> string_to_enum(const std::string& _str) {
    return NamesLiteral::from_string(_str).transform(literal_to_enum);
  }
};

}  // namespace enums
}  // namespace internal
}  // namespace rfl

#endif
