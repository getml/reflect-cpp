#ifndef RFL_UNDERLYINGENUMS_HPP_
#define RFL_UNDERLYINGENUMS_HPP_

namespace rfl {

/// This is a 'fake' processor - it doesn't do much by itself, but its
/// its inclusion instructs parsers not to convert enum types to strings, but to integers
struct UnderlyingEnums {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
