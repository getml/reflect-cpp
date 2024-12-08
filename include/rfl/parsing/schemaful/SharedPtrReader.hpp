#ifndef RFL_PARSING_SCHEMAFUL_SHAREDPTRREADER_HPP_
#define RFL_PARSING_SCHEMAFUL_SHAREDPTRREADER_HPP_

#include <memory>
#include <type_traits>

#include "../../Result.hpp"

namespace rfl::parsing::schemaful {

template <class R, class W, class T, class ProcessorsType>
struct SharedPtrReader {
  static Result<std::shared_ptr<T>> read(
      const R& _r, const size_t _index,
      const typename R::InputVarType& _var) noexcept {
    if (_index == 1) {
      return std::shared_ptr<T>();
    }
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform([](T&& _t) { return std::make_shared<T>(std::move(_t)); });
  }
};

}  // namespace rfl::parsing::schemaful

#endif
