#ifndef RFL_STRIPFIELDNAMES_HPP_
#define RFL_STRIPFIELDNAMES_HPP_

namespace rfl {

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to strip field names.
struct StripFieldNames {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
