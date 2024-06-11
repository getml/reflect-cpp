#ifndef RFL_NOFIELDNAMES_HPP_
#define RFL_NOFIELDNAMES_HPP_

namespace rfl {

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to strip field names.
struct NoFieldNames {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
