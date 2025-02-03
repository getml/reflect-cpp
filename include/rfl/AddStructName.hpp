#ifndef RFL_ADDSTRUCTNAME_HPP_
#define RFL_ADDSTRUCTNAME_HPP_

#include <tuple>

#include "Field.hpp"
#include "Literal.hpp"
#include "apply.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/get_type_name.hpp"
#include "internal/remove_namespaces.hpp"
#include "make_named_tuple.hpp"

namespace rfl {

template <internal::StringLiteral field_name_>
struct AddStructName {
  /// Adds the name of the struct as a new field.
  template <class StructType>
  static auto process(const auto& _view) {
    using LiteralType = Literal<
        internal::remove_namespaces<internal::get_type_name<StructType>()>()>;
    using FieldType = Field<field_name_, LiteralType>;
    const auto add_new_field = [](const auto&... _fields) {
      return make_named_tuple(FieldType(LiteralType()), _fields...);
    };
    return rfl::apply(add_new_field, _view.fields());
  }
};

}  // namespace rfl

#endif
