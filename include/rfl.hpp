#ifndef RFL_RFL_HPP_
#define RFL_RFL_HPP_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4101)
#endif

#include "rfl/AddStructName.hpp"
#include "rfl/AllOf.hpp"
#include "rfl/AnyOf.hpp"
#include "rfl/Attribute.hpp"
#include "rfl/Box.hpp"
#include "rfl/Description.hpp"
#include "rfl/Field.hpp"
#include "rfl/Flatten.hpp"
#include "rfl/Literal.hpp"
#include "rfl/NamedTuple.hpp"
#include "rfl/NoOptionals.hpp"
#include "rfl/OneOf.hpp"
#include "rfl/Pattern.hpp"
#include "rfl/PatternValidator.hpp"
#include "rfl/Processors.hpp"
#include "rfl/Ref.hpp"
#include "rfl/Rename.hpp"
#include "rfl/Size.hpp"
#include "rfl/Skip.hpp"
#include "rfl/SnakeCaseToCamelCase.hpp"
#include "rfl/SnakeCaseToPascalCase.hpp"
#include "rfl/TaggedUnion.hpp"
#include "rfl/Timestamp.hpp"
#include "rfl/Validator.hpp"
#include "rfl/Variant.hpp"
#include "rfl/always_false.hpp"
#include "rfl/as.hpp"
#include "rfl/comparisons.hpp"
#include "rfl/default.hpp"
#include "rfl/define_literal.hpp"
#include "rfl/define_named_tuple.hpp"
#include "rfl/define_tagged_union.hpp"
#include "rfl/define_variant.hpp"
#include "rfl/enums.hpp"
#include "rfl/extract_discriminators.hpp"
#include "rfl/field_type.hpp"
#include "rfl/fields.hpp"
#include "rfl/from_named_tuple.hpp"
#include "rfl/get.hpp"
#include "rfl/make_named_tuple.hpp"
#include "rfl/name_t.hpp"
#include "rfl/named_tuple_t.hpp"
#include "rfl/parsing/CustomParser.hpp"
#include "rfl/patterns.hpp"
#include "rfl/remove_fields.hpp"
#include "rfl/replace.hpp"
#include "rfl/to_named_tuple.hpp"
#include "rfl/to_view.hpp"
#include "rfl/type_name_t.hpp"
#include "rfl/visit.hpp"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
