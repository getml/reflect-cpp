#ifndef RFL_CONFIG_HPP_
#define RFL_CONFIG_HPP_

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

}  // namespace rfl::config

#endif

