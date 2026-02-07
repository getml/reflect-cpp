#ifndef RFL_SNAKECASETOKEBABCASE_HPP_
#define RFL_SNAKECASETOKEBABCASE_HPP_

#include "Field.hpp"
#include "internal/is_rename.hpp"
#include "internal/transform_case.hpp"

namespace rfl {

struct SnakeCaseToKebabCase {
 public:
  /// Replaces all instances of snake_case field names with kebab-case.
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
  template <class FieldType>
  static auto handle_one_field(const FieldType& _f) {
    using NewFieldType =
        Field<internal::transform_snake_to_kebab<FieldType::name_>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
