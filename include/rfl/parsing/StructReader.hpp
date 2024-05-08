#ifndef RFL_PARSING_STRUCTREADER_HPP_
#define RFL_PARSING_STRUCTREADER_HPP_

#include <array>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../to_view.hpp"
#include "AreReaderAndWriter.hpp"

namespace rfl::parsing {

template <class R, class W, class StructType, class ProcessorsType>
requires AreReaderAndWriter<R, W, StructType>
struct StructReader {
 public:
  using InputVarType = typename R::InputVarType;

  /// The way this works is that we allocate space on the stack in this size of
  /// the struct in which we then write the individual fields using
  /// views and placement new. This is how we deal with the fact that some
  /// fields might not be default-constructible.
  static Result<StructType> read(const R& _r, const InputVarType& _var) {
    alignas(StructType) unsigned char buf[sizeof(StructType)];
    auto ptr = reinterpret_cast<StructType*>(buf);
    auto view = ProcessorsType::template process<StructType>(to_view(*ptr));
    using ViewType = std::remove_cvref_t<decltype(view)>;
    const auto err =
        Parser<R, W, ViewType, ProcessorsType>::read_view(_r, _var, &view);
    if (err) {
      return *err;
    }
    return std::move(*ptr);
  }
};

}  // namespace rfl::parsing

#endif
