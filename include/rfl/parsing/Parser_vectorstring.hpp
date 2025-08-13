#ifndef RFL_PARSING_PARSER_VECTORSRING_HPP_
#define RFL_PARSING_PARSER_VECTORSRING_HPP_

#include <map>

#include "../Vectorstring.hpp"
#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Vectorstring>
struct Parser<R, W, Vectorstring, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  static Result<Vectorstring> read(const R& _r,
                                 const InputVarType& _var) noexcept {
    return _r.template to_basic_type<Vectorstring>(_var);
  }

  template <class P>
  static void write(const W& _w, const Vectorstring& _b,
                    const P& _parent) noexcept {
    ParentType::add_value(_w, _b, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return schema::Type{schema::Type::Vectorstring{}};
  }
};

}  // namespace rfl::parsing

#endif
