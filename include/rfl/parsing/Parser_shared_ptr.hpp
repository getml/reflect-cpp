#ifndef RFL_PARSING_PARSER_SHARED_PTR_HPP_
#define RFL_PARSING_PARSER_SHARED_PTR_HPP_

#include <memory>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::shared_ptr<T>>
struct Parser<R, W, std::shared_ptr<T>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

  static Result<std::shared_ptr<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    if (_r.is_empty(*_var)) {
      return std::shared_ptr<T>();
    }
    const auto to_ptr = [](auto&& _t) {
      return std::make_shared<T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>>::read(_r, _var).transform(
        to_ptr);
  }

  template <class P>
  static OutputVarType write(const W& _w, const std::shared_ptr<T>& _s,
                             const P& _parent) noexcept {
    if (!_s) {
      ParentType::add_null(_w, _parent);
      return;
    }
    Parser<R, W, std::remove_cvref_t<T>>::write(_w, *_s, _parent);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
