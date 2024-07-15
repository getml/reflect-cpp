#ifndef RFL_PARSING_SCHEMA_VALIDATIONTYPE_HPP_
#define RFL_PARSING_SCHEMA_VALIDATIONTYPE_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "../../Ref.hpp"
#include "../../Variant.hpp"

namespace rfl::parsing::schema {

struct ValidationType {
  struct AllOf {
    std::vector<ValidationType> types_;
  };

  struct AnyOf {
    std::vector<ValidationType> types_;
  };

  struct EqualTo {
    rfl::Variant<double, int> value_;
  };

  struct ExclusiveMaximum {
    rfl::Variant<double, int> value_;
  };

  struct ExclusiveMinimum {
    rfl::Variant<double, int> value_;
  };

  struct Maximum {
    rfl::Variant<double, int> value_;
  };

  struct Minimum {
    rfl::Variant<double, int> value_;
  };

  struct NotEqualTo {
    rfl::Variant<double, int> value_;
  };

  struct OneOf {
    std::vector<ValidationType> types_;
  };

  struct Regex {
    std::string pattern_;
  };

  struct Size {
    Ref<ValidationType> size_limit_;
  };

  using VariantType =
      rfl::Variant<AllOf, AnyOf, EqualTo, ExclusiveMaximum, ExclusiveMinimum,
                   Maximum, Minimum, OneOf, NotEqualTo, Regex, Size>;

  /// A type can be determined to be any of the above.
  VariantType variant_;
};

}  // namespace rfl::parsing::schema

#endif
