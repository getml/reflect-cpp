#ifndef RFL_ALLOWRAWPTRS_HPP_
#define RFL_ALLOWRAWPTRS_HPP_

namespace rfl {

/// A processor that instructs parsers to allow raw pointers in structs.
/// This is a marker type (doesn't modify data) that changes parser behavior.
/// By default, reflect-cpp does not support raw pointers to avoid memory management issues.
/// When AllowRawPtrs is added as a processor, raw pointers (T*) in structs will be accepted
/// and serialized/deserialized. Use with caution as this can lead to memory leaks or dangling pointers.
/// Usage: rfl::json::read<MyStruct, AllowRawPtrs>(json_str)
struct AllowRawPtrs {
 public:
  /// Identity process function - returns the named tuple unchanged.
  /// The actual raw pointer handling happens in the parser, not here.
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
