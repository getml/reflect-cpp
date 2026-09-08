#ifndef RFL_SNAKECASETOCAMELCASE_HPP_
#define RFL_SNAKECASETOCAMELCASE_HPP_

#include "Field.hpp"
#include "internal/is_rename.hpp"
#include "internal/transform_case.hpp"

namespace rfl {

struct SnakeCaseToCamelCase {
 public:
  /// Replaces all instances of snake_case field names with camelCase.
  /// A named tuple is a tuple-like structure where each element has a name/key.
  /// @tparam StructType The type of the struct being processed
  /// @param _named_tuple The named tuple containing the struct's fields
  /// @return A new named tuple with field names converted to camelCase
  template <class StructType>
  static auto process(const auto& _named_tuple) {
    return _named_tuple.transform([]<class FieldType>(const FieldType& _f) {
      if constexpr (FieldType::name() != "xml_content" &&
                    !internal::is_rename_v<typename FieldType::Type>) {
        return handle_one_field(_f);
      } else {
        return _f;
      }
    });
  }

 private:
  /// Applies the snake_case to camelCase transformation to a single field.
  /// @tparam FieldType The type of the field being transformed
  /// @param _f The field to transform
  /// @return A new field with the transformed name and the same value
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<internal::transform_snake_case<FieldType::name_,
                                             /*capitalize=*/false>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
