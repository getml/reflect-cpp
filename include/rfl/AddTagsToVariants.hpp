#ifndef RFL_ADDTAGSTOVARIANTS_HPP_
#define RFL_ADDTAGSTOVARIANTS_HPP_

namespace rfl {

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to automatically add tags to the variants
/// they might encounter.
struct AddTagsToVariants {
 public:
  /// Processes a named tuple (a tuple-like structure with named fields) without modification.
  /// This method exists to satisfy the processor interface but doesn't transform the data.
  /// @tparam StructType The type of the struct being processed
  /// @param _named_tuple The named tuple representing the struct's fields
  /// @return The unmodified named tuple
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to automatically add tags to the variants
/// they might encounter.
struct AddNamespacedTagsToVariants {
 public:
  /// Processes a named tuple (a tuple-like structure with named fields) without modification.
  /// This method exists to satisfy the processor interface but doesn't transform the data.
  /// @tparam StructType The type of the struct being processed
  /// @param _named_tuple The named tuple representing the struct's fields
  /// @return The unmodified named tuple
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
