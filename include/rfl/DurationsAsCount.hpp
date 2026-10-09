#ifndef RFL_DURATIONSASCOUNT_HPP_
#define RFL_DURATIONSASCOUNT_HPP_

namespace rfl {

/// This is a 'fake' processor - it doesn't do much by itself, but its
/// inclusion instructs parsers to write and read std::chrono::duration types
/// as a plain count in the unit of the C++ type, instead of {count, unit}.
struct DurationsAsCount {
 public:
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return _named_tuple;
  }
};

}  // namespace rfl

#endif
