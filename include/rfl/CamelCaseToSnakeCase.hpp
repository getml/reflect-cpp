#ifndef RFL_CAMELCASETOSNAKECASE_HPP_
#define RFL_CAMELCASETOSNAKECASE_HPP_

#include "Field.hpp"
#include "internal/is_rename.hpp"
#include "internal/transform_case.hpp"

namespace rfl {

struct CamelCaseToSnakeCase {
 public:
  /// Replaces all instances of camelCase field names with snake_case.
  /// A named tuple is a tuple-like structure where each element has a name/key.
  /// @tparam StructType The type of the struct being processed
  /// @param _named_tuple The named tuple containing the struct's fields
  /// @return A new named tuple with field names converted to snake_case
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
  /// Applies the camelCase to snake_case transformation to a single field.
  /// @tparam FieldType The type of the field being transformed
  /// @param _f The field to transform
  /// @return A new field with the transformed name and the same value
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<internal::transform_camel_case<FieldType::name_>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
