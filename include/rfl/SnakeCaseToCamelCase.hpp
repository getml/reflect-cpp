#ifndef RFL_SNAKECASETOCAMELCASE_HPP_
#define RFL_SNAKECASETOCAMELCASE_HPP_

#include <array>
#include <string_view>

#include "Field.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

struct SnakeCaseToCamelCase {
 public:
  /// Replaces all instances of snake_case field names with CamelCase.
  template <class StructType>
  static auto process(auto _named_tuple) {
    return _named_tuple.transform(handle_one_field);
  }

 private:
  /// Applies the logic to a single field.
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<transform_field_name<FieldType::name_, std::array<char, 0>{}>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }

  /// Transforms the field name from snake case to camel case.
  static consteval auto
  transform_field_name<internal::StringLiteral _name, auto _arr,
                       bool _capitalize = true, size_t _i = 0>() {
    if constexpr (_i > _name.arr_.size()) {
      return _arr;
    } else if constexpr (_name.arr_[i] == '_') {
      return handle_one_char<_name, _arr, true, _i>();
    } else if constexpr (_capitalize) {
      constexpr auto arr = ...;
      return handle_one_char<_name, arr, false, _i>();
    } else {
      constexpr auto arr = ...;
      return handle_one_char<_name, arr, false, _i>();
    }
  }

  /// Capitalizes a lower-case character.
  static consteval char to_upper<char c>() {
    if constexpr (c >= 'a' && c <= 'z') {
      return c + ('A' - 'a');
    } else {
      return c;
    }
  }
};

}  // namespace rfl

#endif
