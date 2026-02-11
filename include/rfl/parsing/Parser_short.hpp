#ifndef RFL_PARSING_PARSER_SHORT_HPP_
#define RFL_PARSING_PARSER_SHORT_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../Short.hpp"
#include "../internal/StringLiteral.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, internal::StringLiteral _name,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, Short<_name, T>>
struct Parser<R, W, Short<_name, T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<Short<_name, T>> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    const auto to_short = [](auto&& _t) {
      return Short<_name, T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_short);
  }

  template <class P>
  static void write(const W& _w, const Short<_name, T>& _short,
                    const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _short.value(), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
