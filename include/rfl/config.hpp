#ifndef RFL_CONFIG_HPP_
#define RFL_CONFIG_HPP_

#include <string_view>

namespace rfl::config {

// To specify a different range for a particular enum type, specialize the
// enum_range template for that enum type.
template <typename T>
struct enum_range {
  // In your template specialization, uncomment these two lines and replace them
  // with the values of your choice.
  // static constexpr int min = ...;
  // static constexpr int max = ...;
};

// To add descriptions to enum values for JSON schema generation, specialize
// the enum_descriptions template for that enum type.
// Example:
//   template <>
//   struct rfl::config::enum_descriptions<MyEnum> {
//     static constexpr std::string_view get(MyEnum value) {
//       switch (value) {
//         case MyEnum::option1: return "Description for option1";
//         case MyEnum::option2: return "Description for option2";
//         default: return "";
//       }
//     }
//   };
template <typename T>
struct enum_descriptions {
  // Default implementation returns empty string (no descriptions)
  static constexpr std::string_view get(T) { return ""; }
  // Set to true in specializations that provide descriptions
  static constexpr bool has_descriptions = false;
};

}  // namespace rfl::config

#endif

