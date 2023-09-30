#ifndef RFL_DEFAULT_HPP_
#define RFL_DEFAULT_HPP_

namespace rfl {

/// Helper class that can be passed to a field
/// to trigger the default value of the type.
struct Default {};

inline static const auto default_value = Default{};

}  // namespace rfl

#endif
