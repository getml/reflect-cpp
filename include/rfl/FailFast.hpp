#ifndef RFL_FAILFAST_HPP_
#define RFL_FAILFAST_HPP_

#include <utility>

namespace rfl {

/// Report only the first object field error and skip decoding later fields.
/// For example: rfl::json::read<MyStruct, rfl::FailFast>(input).
/// Readers may still traverse the input after a field error.
struct FailFast {
  template <class StructType>
  static auto process(auto&& _named_tuple) {
    return std::forward<decltype(_named_tuple)>(_named_tuple);
  }
};

}  // namespace rfl

#endif
