#ifndef RFL_NOFIELDNAMES_HPP_
#define RFL_NOFIELDNAMES_HPP_

namespace rfl {

/// A processor that instructs parsers to strip field names during serialization.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// When included as a processor, output will be positional/array-based instead of named.
/// For example, JSON output becomes [value1, value2] instead of {"field1": value1, "field2": value2}.
/// Note: Not supported for BSON, XML, TOML, or YAML formats.
/// Usage: rfl::json::write<NoFieldNames>(my_struct)
struct NoFieldNames {
 public:
  /// Identity process function - returns the named tuple unchanged.
  /// The actual stripping happens in the writer, not here.
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
