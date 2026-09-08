#ifndef RFL_NOOPTIONALS_HPP_
#define RFL_NOOPTIONALS_HPP_

namespace rfl {

/// A processor that instructs parsers to require all fields be present in input.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// By default, optional fields (std::optional, rfl::Box with defaults) can be omitted from input.
/// Including NoOptionals as a processor makes all fields mandatory, even those that would normally be optional.
/// Usage: rfl::json::read<MyStruct, NoOptionals>(json_str)
struct NoOptionals {
 public:
  /// Identity process function - returns the named tuple unchanged.
  /// The actual requirement enforcement happens in the parser, not here.
  /// @tparam StructType The struct type being processed
  /// @param _named_tuple The named tuple representation of the struct
  /// @return The same named tuple (unchanged)
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
