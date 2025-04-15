#ifndef RFL_PARSING_PARSER_BYTESTRING_HPP_
#define RFL_PARSING_PARSER_BYTESTRING_HPP_

#include <map>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Bytestring>
struct Parser<R, W, Bytestring, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  static Result<Bytestring> read(const R& _r,
                                 const InputVarType& _var) noexcept {
    return _r.template to_basic_type<Bytestring>(_var);
  }

  template <class P>
  static void write(const W& _w, const Bytestring& _b,
                    const P& _parent) noexcept {
    ParentType::add_value(_w, _b, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return schema::Type{schema::Type::Bytestring{}};
  }
};

}  // namespace rfl::parsing

#endif
