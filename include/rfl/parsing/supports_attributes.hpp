#ifndef RFL_PARSING_SUPPORTSATTRIBUTES_HPP_
#define RFL_PARSING_SUPPORTSATTRIBUTES_HPP_

#include <concepts>

#include "../Result.hpp"

namespace rfl {
namespace parsing {

/// Determines whether a writer supports attributes.
template <class W>
concept supports_attributes = requires(W w, int val, bool _is_attribute) {
  {
    w.from_basic_type(val, _is_attribute)
    } -> std::same_as<typename W::OutputVarType>;
};

}  // namespace parsing
}  // namespace rfl

#endif
