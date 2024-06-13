#ifndef RFL_PARSING_PARSER_BOX_HPP_
#define RFL_PARSING_PARSER_BOX_HPP_

#include <map>
#include <type_traits>

#include "../Box.hpp"
#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, Box<T>>
struct Parser<R, W, Box<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<Box<T>> read(const R& _r, const InputVarType& _var) noexcept {
    const auto to_box = [](auto&& _t) { return Box<T>::make(std::move(_t)); };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_box);
  }

  template <class P>
  static void write(const W& _w, const Box<T>& _box,
                    const P& _parent) noexcept {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_box,
                                                                _parent);
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
