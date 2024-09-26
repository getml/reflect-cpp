#ifndef RFL_NOEXTRAFIELDS_HPP_
#define RFL_NOEXTRAFIELDS_HPP_

namespace rfl {

/// This is a "fake" processor - it doesn't do much in itself, but its
/// inclusion instructs the parsers to return an error when there are extra
/// fields instead of ignoring them.
struct NoExtraFields {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
