#ifndef RFL_PARSING_PARSER_OPTIONAL_HPP_
#define RFL_PARSING_PARSER_OPTIONAL_HPP_

#include <optional>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
  requires AreReaderAndWriter<R, W, std::optional<T>>
struct Parser<R, W, std::optional<T>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<std::optional<T>> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    if (_r.is_empty(_var)) {
      return std::optional<T>();
    }
    const auto to_opt = [](auto&& _t) { return std::make_optional<T>(_t); };
    return Parser<R, W, std::remove_cvref_t<T>>::read(_r, _var).transform(
        to_opt);
  }

  static OutputVarType write(const W& _w, const std::optional<T>& _o) noexcept {
    if (!_o) {
      return _w.empty_var();
    }
    return Parser<R, W, std::remove_cvref_t<T>>::write(_w, *_o);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
