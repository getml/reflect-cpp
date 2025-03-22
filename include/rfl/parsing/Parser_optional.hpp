#ifndef RFL_PARSING_PARSER_OPTIONAL_HPP_
#define RFL_PARSING_PARSER_OPTIONAL_HPP_

#include <map>
#include <optional>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/OptionalReader.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::optional<T>>
struct Parser<R, W, std::optional<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<std::optional<T>> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    if constexpr (schemaful::IsSchemafulReader<R>) {
      using O = schemaful::OptionalReader<R, W, std::remove_cvref_t<T>,
                                          ProcessorsType>;
      const auto to_optional = [&](const auto& _u) -> Result<std::optional<T>> {
        return _r.template read_union<std::optional<T>, O>(_u);
      };
      return _r.to_union(_var).and_then(to_optional);
    } else {
      if (_r.is_empty(_var)) {
        return std::optional<T>();
      }
      const auto to_opt = [](auto&& _t) { return std::make_optional<T>(std::forward<decltype(_t)>(_t)); };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_opt);
    }
  }

  template <class P>
  static void write(const W& _w, const std::optional<T>& _o,
                    const P& _parent) noexcept {
    if constexpr (schemaful::IsSchemafulWriter<W>) {
      auto u = ParentType::add_union(_w, _parent);
      using UnionType = typename ParentType::template Union<decltype(u)>;
      auto p =
          UnionType{.index_ = static_cast<size_t>(_o ? 0 : 1), .union_ = &u};
      if (_o) {
        Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_o, p);
      } else {
        ParentType::add_null(_w, p);
      }
    } else {
      if (_o) {
        Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_o,
                                                                    _parent);
      } else {
        ParentType::add_null(_w, _parent);
      }
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{schema::Type::Optional{Ref<schema::Type>::make(
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions))}};
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
