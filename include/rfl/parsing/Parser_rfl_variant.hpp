#ifndef RFL_PARSING_PARSER_RFL_VARIANT_HPP_
#define RFL_PARSING_PARSER_RFL_VARIANT_HPP_

#include <map>
#include <type_traits>
#include <variant>

#include "../Result.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"
#include "FieldVariantParser.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes, class ProcessorsType>
requires AreReaderAndWriter<R, W, rfl::Variant<FieldTypes...>>
struct Parser<R, W, rfl::Variant<FieldTypes...>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  /// Expresses the variables as type T.
  static Result<rfl::Variant<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto to_rfl_variant = [](auto&& _var) {
      return rfl::Variant<FieldTypes...>(
          std::forward<std::variant<FieldTypes...>>(_var));
    };
    return Parser<R, W, std::variant<FieldTypes...>, ProcessorsType>::read(_r,
                                                                           _var)
        .transform(to_rfl_variant);
  }

  template <class P>
  static void write(const W& _w, const rfl::Variant<FieldTypes...>& _variant,
                    const P& _parent) noexcept {
    Parser<R, W, std::variant<FieldTypes...>, ProcessorsType>::write(
        _w, _variant.variant(), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::variant<FieldTypes...>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
