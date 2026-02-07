#ifndef RFL_PARSING_PARSER_COMMENTED_HPP_
#define RFL_PARSING_PARSER_COMMENTED_HPP_

#include <map>
#include <optional>
#include <type_traits>

#include "../Commented.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/OptionalReader.hpp"
#include "supports_comments.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Commented<T>>
struct Parser<R, W, Commented<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<Commented<T>> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    if constexpr (supports_comments<R>) {
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform([&](auto&& _t) {
            return Commented<T>(std::move(_t), _r.get_comment(_var));
          });
    } else {
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform([](auto&& _t) {
            return Commented<T>(std::forward<decltype(_t)>(_t));
          });
    }
  }

  template <class P>
  static void write(const W& _w, const Commented<T>& _c, const P& _parent) {
    if constexpr (supports_comments<W>) {
      if (_c.comment()) {
        _w.add_comment(_parent, *_c.comment());
      }
    }
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _c.get(),
                                                                _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{schema::Type::Optional{Ref<schema::Type>::make(
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions))}};
  }
};

}  // namespace rfl::parsing

#endif
