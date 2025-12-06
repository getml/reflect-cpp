#ifndef RFL_PARSING_PARSER_DEFAULTVAL_HPP_
#define RFL_PARSING_PARSER_DEFAULTVAL_HPP_

#include <map>
#include <type_traits>

#include "../DefaultVal.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, DefaultVal<T>>
struct Parser<R, W, DefaultVal<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<DefaultVal<T>> read(const R& _r,
                                    const InputVarType& _var) noexcept {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform([](auto&& _t) {
          return DefaultVal<T>(std::forward<decltype(_t)>(_t));
        });
  }

  template <class P>
  static void write(const W& _w, const DefaultVal<T>& _d, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _d.value(),
                                                                _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions)};
  }
};

}  // namespace rfl::parsing

#endif
