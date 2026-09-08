#ifndef RFL_ENUMNAMESONLY_HPP_
#define RFL_ENUMNAMESONLY_HPP_

namespace rfl {

/// A processor that instructs parsers to accept only the declared names of an
/// enum's enumerators when reading an enum from a string.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// By default, when reading an enum from a string, numeric values are accepted
/// in addition to the declared enumerator names, even when they do not
/// correspond to a declared enumerator (for instance, reading "4" into an enum
/// with values 1, 2 and 3 will produce the cast value 4).
/// When EnumNamesOnly is added as a processor, numeric values are rejected and
/// only the declared enumerator names will be accepted.
/// Usage: rfl::json::read<MyStruct, EnumNamesOnly>(json_str)
struct EnumNamesOnly {
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
