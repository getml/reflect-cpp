#ifndef RFL_PARSING_PARSER_RENAME_HPP_
#define RFL_PARSING_PARSER_RENAME_HPP_

#include <type_traits>

#include "rfl/Rename.hpp"
#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, internal::StringLiteral _name>
requires AreReaderAndWriter<R, W, Rename<_name, T>>
struct Parser<R, W, Rename<_name, T>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<Rename<_name, T>> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    const auto to_rename = [](auto&& _t) {
      return Rename<_name, T>(std::move(_t));
    };
    return Parser<R, W, std::decay_t<T>>::read(_r, _var).transform(to_rename);
  }

  static OutputVarType write(const W& _w,
                             const Rename<_name, T>& _rename) noexcept {
    return Parser<R, W, std::decay_t<T>>::write(_w, _rename.value());
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
