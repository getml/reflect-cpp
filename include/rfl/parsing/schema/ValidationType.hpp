#ifndef RFL_PARSING_SCHEMA_VALIDATIONTYPE_HPP_
#define RFL_PARSING_SCHEMA_VALIDATIONTYPE_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "../../Ref.hpp"

namespace rfl::parsing::schema {

struct ValidationType {
  struct AllOf {
    std::vector<ValidationType> types_;
  };

  struct AnyOf {
    std::vector<ValidationType> types_;
  };

  struct EqualTo {
    std::variant<double, int> value_;
  };

  struct ExclusiveMaximum {
    std::variant<double, int> value_;
  };

  struct ExclusiveMinimum {
    std::variant<double, int> value_;
  };

  struct Maximum {
    std::variant<double, int> value_;
  };

  struct Minimum {
    std::variant<double, int> value_;
  };

  struct NotEqualTo {
    std::variant<double, int> value_;
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
      std::variant<AllOf, AnyOf, EqualTo, ExclusiveMaximum, ExclusiveMinimum,
                   Maximum, Minimum, OneOf, NotEqualTo, Regex, Size>;

  /// A type can be determined to be any of the above.
  VariantType variant_;
};

}  // namespace rfl::parsing::schema

#endif
