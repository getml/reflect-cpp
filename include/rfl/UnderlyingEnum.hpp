#ifndef RFL_UNDERLYINGENUM_HPP_
#define RFL_UNDERLYINGENUM_HPP_

namespace rfl {

/// This is a ‘fake’ processor - it doesn't do much by itself, but its
/// its inclusion instructs parsers not to convert enum types to strings, but to integers
struct UnderlyingEnum {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
