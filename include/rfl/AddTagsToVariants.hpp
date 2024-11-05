#ifndef RFL_ADDTAGSTOVARIANTS_HPP_
#define RFL_ADDTAGSTOVARIANTS_HPP_

namespace rfl {

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to automatically add tags to the variants
/// they might encounter.
struct AddTagsToVariants {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
