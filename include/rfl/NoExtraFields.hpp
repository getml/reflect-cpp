#ifndef RFL_NOEXTRAFIELDS_HPP_
#define RFL_NOEXTRAFIELDS_HPP_

namespace rfl {

/// A processor that instructs parsers to return an error when extra fields are present.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// By default, parsers ignore unknown fields in serialized data. Including NoExtraFields
/// as a processor will cause parsing to fail if the input contains fields not in the struct.
/// Usage: rfl::json::read<MyStruct, NoExtraFields>(json_str)
struct NoExtraFields {
 public:
  /// Identity process function - returns the named tuple unchanged.
  /// The actual validation happens in the parser, not here.
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
