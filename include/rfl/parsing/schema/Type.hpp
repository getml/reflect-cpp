#ifndef RFL_PARSING_SCHEMA_TYPE_HPP_
#define RFL_PARSING_SCHEMA_TYPE_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "../../Ref.hpp"
#include "ValidationType.hpp"

namespace rfl::parsing::schema {

struct Type {
  struct Boolean {};

  struct Int32 {};

  struct Int64 {};

  struct UInt32 {};

  struct UInt64 {};

  struct Integer {};

  struct Float {};

  struct Double {};

  struct String {};

  struct AnyOf {
    std::vector<Type> types_;
  };

  struct Description {
    std::string description_;
    Ref<Type> type_;
  };

  struct FixedSizeTypedArray {
    size_t size_;
    Ref<Type> type_;
  };

  struct Literal {
    std::vector<std::string> values_;
  };

  struct Object {
    std::map<std::string, Type> types_;
  };

  /// All values are assumed to be required unless explicitly stated otherwise
  /// using this wrapper.
  struct Optional {
    Ref<Type> type_;
  };

  /// The is necessary to resolve circular definitions. Refers to something in
  /// Definitions.
  struct Reference {
    std::string name_;
  };

  // A map with key type string.
  struct StringMap {
    Ref<Type> value_type_;
  };

  struct Tuple {
    std::vector<Type> types_;
  };

  struct TypedArray {
    Ref<Type> type_;
  };

  struct Validated {
    Ref<Type> type_;
    ValidationType validation_;
  };

  using VariantType =
      std::variant<Boolean, Int32, Int64, UInt32, UInt64, Integer, Float,
                   Double, String, AnyOf, Description, FixedSizeTypedArray,
                   Literal, Object, Optional, Reference, StringMap, Tuple,
                   TypedArray, Validated>;

  /// A type can be determined to be any of the above.
  VariantType variant_;
};

}  // namespace rfl::parsing::schema

#endif
