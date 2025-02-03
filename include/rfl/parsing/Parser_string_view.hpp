#ifndef RFL_PARSING_PARSER_STRING_VIEW_HPP_
#define RFL_PARSING_PARSER_STRING_VIEW_HPP_

#include <map>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::string_view>
struct Parser<R, W, std::string_view, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<std::string_view> read(const R&, const InputVarType&) noexcept {
    static_assert(always_false_v<R>,
                  "Reading into std::string_view is dangerous and "
                  "therefore unsupported. "
                  "Please consider using std::string instead.");
    return error("Unsupported.");
  }

  template <class P>
  static void write(const W& _w, const std::string_view& _str,
                    const P& _p) noexcept {
    Parser<R, W, std::string, ProcessorsType>::write(_w, std::string(_str), _p);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::string, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
