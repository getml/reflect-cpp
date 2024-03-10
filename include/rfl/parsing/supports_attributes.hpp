#ifndef RFL_PARSING_SUPPORTSATTRIBUTES_HPP_
#define RFL_PARSING_SUPPORTSATTRIBUTES_HPP_

#include <concepts>
#include <string>
#include <string_view>

#include "../Result.hpp"

namespace rfl {
namespace parsing {

/// Determines whether a writer supports attributes.
template <class W>
concept supports_attributes = requires(W w, std::string_view name,
                                       typename W::OutputObjectType obj,
                                       bool is_attribute) {
  {
    w.add_value_to_object(name, name, &obj, is_attribute)
    } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_null_to_object(name, &obj, is_attribute)
    } -> std::same_as<typename W::OutputVarType>;
};

}  // namespace parsing
}  // namespace rfl

#endif
