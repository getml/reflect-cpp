#ifndef RFL_PARSING_SCHEMAFUL_UNIQUEPTRREADER_HPP_
#define RFL_PARSING_SCHEMAFUL_UNIQUEPTRREADER_HPP_

#include <memory>
#include <type_traits>

#include "../../Result.hpp"
#include "../Parser_base.hpp"

namespace rfl::parsing::schemaful {

template <class R, class W, class T, class ProcessorsType>
struct UniquePtrReader {
  /**
   * @brief Reads a unique_ptr from the input.
   *
   * @param _r The reader to use.
   * @param _index The index of the alternative to read (0 for value, 1 for null).
   * @param _var The input variable to read from.
   * @return A Result containing the parsed unique_ptr or an error.
   */
  static Result<std::unique_ptr<T>> read(
      const R& _r, const size_t _index,
      const typename R::InputVarType& _var) noexcept {
    if (_index == 1) {
      return std::unique_ptr<T>();
    }
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform([](T&& _t) { return std::make_unique<T>(std::move(_t)); });
  }
};

}  // namespace rfl::parsing::schemaful

#endif
