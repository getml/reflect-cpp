#ifndef RFL_PARSING_PARSER_RFL_VARIANT_HPP_
#define RFL_PARSING_PARSER_RFL_VARIANT_HPP_

#include <type_traits>
#include <variant>

#include "rfl/Result.hpp"
#include "rfl/Variant.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/FieldVariantParser.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes>
requires AreReaderAndWriter<R, W, rfl::Variant<FieldTypes...>>
struct Parser<R, W, rfl::Variant<FieldTypes...>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  /// Expresses the variables as type T.
  static Result<rfl::Variant<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto to_rfl_variant = [](auto&& _var) {
      return rfl::Variant<FieldTypes...>(
          std::forward<std::variant<FieldTypes...>>(_var));
    };
    return Parser<R, W, std::variant<FieldTypes...>>::read(_r, _var).transform(
        to_rfl_variant);
  }

  static OutputVarType write(
      const W& _w, const rfl::Variant<FieldTypes...>& _variant) noexcept {
    return Parser<R, W, std::variant<FieldTypes...>>::write(_w,
                                                            _variant.variant());
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
