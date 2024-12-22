#ifndef RFL_PARSING_SUPPORTSTAGGEDUNIONS_HPP_
#define RFL_PARSING_SUPPORTSTAGGEDUNIONS_HPP_

#include <array>
#include <concepts>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "../Result.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"

namespace rfl::parsing {

template <class R>
concept SupportsTaggedUnions = requires(
    R r, std::string name, typename R::InputArrayType arr,
    typename R::InputObjectType obj, typename R::InputVarType var, size_t idx) {
  /// Retrieves a particular field from an array.
  {
    r.get_field_from_array(idx, arr)
  } -> std::same_as<rfl::Result<typename R::InputVarType>>;

  /// Retrieves a particular field from an object.
  {
    r.get_field_from_object(name, obj)
  } -> std::same_as<rfl::Result<typename R::InputVarType>>;
};

}  // namespace rfl::parsing

#endif
