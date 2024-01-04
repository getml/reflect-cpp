#ifndef RFL_PARSING_PARSER_VARIANT_HPP_
#define RFL_PARSING_PARSER_VARIANT_HPP_

#include <type_traits>
#include <variant>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "FieldVariantParser.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes>
requires AreReaderAndWriter<R, W, std::variant<FieldTypes...>>
struct Parser<R, W, std::variant<FieldTypes...>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  template <int _i = 0>
  static Result<std::variant<FieldTypes...>> read(
      const R& _r, const InputVarType& _var,
      const std::string _errors = "") noexcept {
    if constexpr (_i == 0 &&
                  internal::all_fields<std::tuple<FieldTypes...>>()) {
      return FieldVariantParser<R, W, FieldTypes...>::read(_r, _var);
    } else if constexpr (_i == sizeof...(FieldTypes)) {
      return Error("Could not parse variant: " + _errors);
    } else {
      const auto to_variant = [](auto&& _val) {
        return std::variant<FieldTypes...>(std::move(_val));
      };

      const auto try_next = [&_r, _var, &_errors](const auto& _err) {
        auto errors = _errors;
        errors += std::string("\n" + std::to_string(_i + 1) + ") ") +
                  internal::strings::replace_all(_err.what(), "\n", "\n    ");
        return read<_i + 1>(_r, _var, errors);
      };

      using AltType = std::remove_cvref_t<
          std::variant_alternative_t<_i, std::variant<FieldTypes...>>>;

      return Parser<R, W, AltType>::read(_r, _var)
          .transform(to_variant)
          .or_else(try_next);
    }
  }

  template <class P>
  static void write(const W& _w, const std::variant<FieldTypes...>& _variant,
                    const P& _parent) noexcept {
    if constexpr (internal::all_fields<std::tuple<FieldTypes...>>()) {
      FieldVariantParser<R, W, FieldTypes...>::write(_w, _variant, _parent);
    } else {
      const auto handle = [&](const auto& _v) {
        using Type = std::remove_cvref_t<decltype(_v)>;
        Parser<R, W, Type>::write(_w, _v, _parent);
      };
      return std::visit(handle, _variant);
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
