#ifndef RFL_DEFAULTIFMISSING_HPP_
#define RFL_DEFAULTIFMISSING_HPP_

namespace rfl {

/// A processor that instructs parsers to use default values for missing fields.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// When a field is missing from input and has a default value, that default will be used
/// instead of treating it as an error. Works with fields that have default constructors,
/// rfl::default_value, or are wrapped in DefaultVal.
/// Usage: rfl::json::read<MyStruct, DefaultIfMissing>(json_str)
struct DefaultIfMissing {
 public:
  /// Identity process function - returns the named tuple unchanged.
  /// The actual default value application happens in the parser, not here.
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
