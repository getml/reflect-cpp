#ifndef RFL_PARSING_SCHEMAFUL_OPTIONALREADER_HPP_
#define RFL_PARSING_SCHEMAFUL_OPTIONALREADER_HPP_

#include <optional>
#include <type_traits>

#include "../../Result.hpp"

namespace rfl::parsing::schemaful {

template <class R, class W, class T, class ProcessorsType>
struct OptionalReader {
  static Result<std::optional<T>> read(
      const R& _r, const size_t _index,
      const typename R::InputVarType& _var) noexcept {
    if (_index == 1) {
      return std::optional<T>();
    }
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var);
  }
};

}  // namespace rfl::parsing::schemaful

#endif
