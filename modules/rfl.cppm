module;

#include "rfl.hpp"
#include "rfl/Deprecated.hpp"
#include "rfl/num_fields.hpp"

export module rfl;

export namespace rfl {
using ::rfl::AddNamespacedTagsToVariants;
using ::rfl::AddStructName;
using ::rfl::AddTagsToVariants;
using ::rfl::AllOf;
using ::rfl::AllowRawPtrs;
using ::rfl::AlphaNumeric;
using ::rfl::AnyOf;
using ::rfl::Attribute;
using ::rfl::Binary;
using ::rfl::Box;
using ::rfl::Bytestring;
using ::rfl::CamelCaseToSnakeCase;
using ::rfl::Commented;
using ::rfl::CopyableBox;
using ::rfl::DefaultIfMissing;
using ::rfl::DefaultVal;
using ::rfl::Deprecated;
using ::rfl::Description;
using ::rfl::Email;
using ::rfl::EqualTo;
using ::rfl::Error;
using ::rfl::ExtraFields;
using ::rfl::Field;
using ::rfl::Flatten;
using ::rfl::Generic;
using ::rfl::Hex;
using ::rfl::Literal;
using ::rfl::Maximum;
using ::rfl::Minimum;
using ::rfl::NamedTuple;
using ::rfl::NoExtraFields;
using ::rfl::NoFieldNames;
using ::rfl::NoOptionals;
using ::rfl::Object;
using ::rfl::Oct;
using ::rfl::OneOf;
using ::rfl::PatternValidator;
using ::rfl::Positional;
using ::rfl::Processors;
using ::rfl::Ref;
using ::rfl::Reflector;
using ::rfl::Rename;
using ::rfl::Result;
using ::rfl::Short;
using ::rfl::Size;
using ::rfl::Skip;
using ::rfl::SnakeCaseToCamelCase;
using ::rfl::SnakeCaseToPascalCase;
using ::rfl::TaggedUnion;
using ::rfl::Timestamp;
using ::rfl::Tuple;
using ::rfl::UnderlyingEnums;
using ::rfl::UUIDv4;
using ::rfl::Validator;
using ::rfl::Variant;
using ::rfl::Vectorstring;
using ::rfl::as;
using ::rfl::enum_to_string;
using ::rfl::error;
using ::rfl::fields;
using ::rfl::from_generic;
using ::rfl::from_named_tuple;
using ::rfl::get;
using ::rfl::get_enum_range;
using ::rfl::get_enumerator_array;
using ::rfl::get_enumerators;
using ::rfl::get_if;
using ::rfl::get_underlying_enumerator_array;
using ::rfl::get_underlying_enumerators;
using ::rfl::holds_alternative;
using ::rfl::make_box;
using ::rfl::make_field;
using ::rfl::make_named_tuple;
using ::rfl::make_ref;
using ::rfl::named_tuple_t;
using ::rfl::num_fields;
using ::rfl::operator<=>;
using ::rfl::operator*;
using ::rfl::operator==;
using ::rfl::replace;
using ::rfl::string_to_enum;
using ::rfl::to_generic;
using ::rfl::to_named_tuple;
using ::rfl::to_view;
using ::rfl::tuple_element_t;
using ::rfl::visit;
}

export namespace rfl::atomic {
using ::rfl::atomic::remove_atomic_t;
using ::rfl::atomic::set_atomic;
}

export namespace rfl::concepts {
using ::rfl::concepts::BackInsertableByteContainer;
using ::rfl::concepts::ByteLike;
using ::rfl::concepts::ContiguousByteContainer;
using ::rfl::concepts::MutableContiguousByteContainer;
}

export namespace rfl::config {
using ::rfl::config::enum_descriptions;
using ::rfl::config::enum_range;
}

export namespace rfl::internal {
using ::rfl::internal::Skip;
using ::rfl::internal::StringLiteral;
using ::rfl::internal::has_default_val_v;
}

export namespace rfl::internal::enums {
using ::rfl::internal::enums::range_defined;
}

export namespace rfl::parsing {
using ::rfl::parsing::CustomParser;
using ::rfl::parsing::Parser;
}
