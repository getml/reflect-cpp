#ifndef RFL_SNAKECASETOCAMELCASE_HPP_
#define RFL_SNAKECASETOCAMELCASE_HPP_

#include <array>
#include <string_view>

#include "Field.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {
namespace internal {
/// Capitalizes a lower-case character.
template <char c>
consteval char to_upper() {
  if constexpr (c >= 'a' && c <= 'z') {
    return c + ('A' - 'a');
  } else {
    return c;
  }
}

/// Transforms the field name from snake case to camel case.
template <internal::StringLiteral _name, bool _capitalize, size_t _i = 0,
          char... chars>
consteval auto transform_field_name() {
  if constexpr (_i == _name.arr_.size()) {
    return StringLiteral<sizeof...(chars) + 1>(chars...);
  } else if constexpr (_name.arr_[_i] == '_') {
    return transform_field_name<_name, true, _i + 1, chars...>();
  } else if constexpr (_capitalize) {
    return transform_field_name<_name, false, _i + 1, chars...,
                                to_upper<_name.arr_[_i]>()>();
  } else {
    return transform_field_name<_name, false, _i + 1, chars...,
                                _name.arr_[_i]>();
  }
}
}  // namespace internal

struct SnakeCaseToCamelCase {
 public:
  /// Replaces all instances of snake_case field names with CamelCase.
  template <class StructType>
  static auto process(auto _named_tuple) {
    const auto handle_one = [](const auto& _f) { return handle_one_field(_f); };
    return _named_tuple.transform(handle_one);
  }

 private:
  /// Applies the logic to a single field.
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<internal::transform_field_name<FieldType::name_, true>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
