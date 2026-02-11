#ifndef RFL_PARSING_PARSER_POSITIONAL_HPP_
#define RFL_PARSING_PARSER_POSITIONAL_HPP_

#include <map>
#include <type_traits>

#include "../Positional.hpp"
#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Positional<T>>
struct Parser<R, W, Positional<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<Positional<T>> read(const R& _r,
                                     const InputVarType& _var) noexcept {
    const auto to_positional = [](auto&& _t) {
      return Positional<T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_positional);
  }

  template <class P>
  static void write(const W& _w, const Positional<T>& _positional,
                    const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _positional.value(), _parent);
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
