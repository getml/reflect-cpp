#ifndef RFL_DEFINEVARIANT_HPP_
#define RFL_DEFINEVARIANT_HPP_

#include <variant>

#include "internal/define_variant.hpp"

namespace rfl {

/// Combines several variant types into a single union variant type.
/// This merges multiple std::variant types into one variant containing all the alternatives.
/// For example, combining std::variant<int, double> and std::variant<std::string, bool>
/// results in std::variant<int, double, std::string, bool>.
/// @tparam Vars The variant types to combine into a single union variant type
template <class... Vars>
using define_variant_t = typename internal::define_variant<Vars...>::type;

}  // namespace rfl

#endif  // RFL_DEFINEVARIANT_HPP_
