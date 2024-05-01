#ifndef RFL_SNAKECASETOCAMELCASE_HPP_
#define RFL_SNAKECASETOCAMELCASE_HPP_

#include "Field.hpp"
#include "internal/transform_snake_case.hpp"

namespace rfl {

struct SnakeCaseToCamelCase {
 public:
  /// Replaces all instances of snake_case field names with camelCase.
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    const auto handle_one = []<class FieldType>(FieldType&& _f) {
      if constexpr (FieldType::name() != "xml_content") {
        return handle_one_field(std::move(_f));
      } else {
        return std::move(_f);
      }
    };
    return _named_tuple.transform(handle_one);
  }

 private:
  /// Applies the logic to a single field.
  template <class FieldType>
  static auto handle_one_field(FieldType&& _f) {
    using NewFieldType =
        Field<internal::transform_snake_case<FieldType::name_,
                                             /*capitalize=*/false>(),
              typename FieldType::Type>;
    return NewFieldType(_f.value());
  }
};

}  // namespace rfl

#endif
