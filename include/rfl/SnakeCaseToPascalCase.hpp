#ifndef RFL_SNAKECASETOPASCALCASE_HPP_
#define RFL_SNAKECASETOPASCALCASE_HPP_

#include "Field.hpp"
#include "internal/is_rename.hpp"
#include "internal/transform_snake_case.hpp"

namespace rfl {

struct SnakeCaseToPascalCase {
 public:
  /// Replaces all instances of snake_case field names with PascalCase.
  template <class StructType>
  static auto process(const auto& _named_tuple) {
    const auto handle_one = []<class FieldType>(const FieldType& _f) {
      if constexpr (FieldType::name() != "xml_content" &&
                    !internal::is_rename_v<typename FieldType::Type>) {
        return handle_one_field(_f);
      } else {
        return _f;
      }
    };
    return _named_tuple.transform(handle_one);
  }

 private:
  /// Applies the logic to a single field.
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<internal::transform_snake_case<FieldType::name_,
                                             /*capitalize=*/true>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
