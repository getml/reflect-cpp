#ifndef RFL_PARSING_PARSER_RFL_VARIANT_HPP_
#define RFL_PARSING_PARSER_RFL_VARIANT_HPP_

#include <map>
#include <type_traits>
#include <variant>

#include "../Result.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"
#include "../internal/nth_element_t.hpp"
#include "FieldVariantParser.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class... AlternativeTypes, class ProcessorsType>
requires AreReaderAndWriter<R, W, rfl::Variant<AlternativeTypes...>>
class Parser<R, W, rfl::Variant<AlternativeTypes...>, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<rfl::Variant<AlternativeTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      return FieldVariantParser<R, W, ProcessorsType,
                                AlternativeTypes...>::read(_r, _var);
    } else {
      std::optional<rfl::Variant<AlternativeTypes...>> result;
      std::vector<Error> errors;
      read_variant(
          _r, _var, &result, &errors,
          std::make_integer_sequence<int, sizeof...(AlternativeTypes)>());
      if (result) {
        return std::move(*result);
      } else {
        return Error(
            to_single_error_message(errors,
                                    "Could not parse the variant. Each of the "
                                    "possible alternatives failed "
                                    "for the following reasons: ",
                                    100000));
      }
    }
  }

  template <class P>
  static void write(const W& _w,
                    const rfl::Variant<AlternativeTypes...>& _variant,
                    const P& _parent) noexcept {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      FieldVariantParser<R, W, ProcessorsType, AlternativeTypes...>::write(
          _w, _variant, _parent);
    } else {
      const auto handle = [&](const auto& _v) {
        using Type = std::remove_cvref_t<decltype(_v)>;
        Parser<R, W, Type, ProcessorsType>::write(_w, _v, _parent);
      };
      return rfl::visit(handle, _variant);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      return FieldVariantParser<R, W, ProcessorsType,
                                AlternativeTypes...>::to_schema(_definitions);
    } else {
      std::vector<schema::Type> types;
      build_schema(
          _definitions, &types,
          std::make_integer_sequence<int, sizeof...(AlternativeTypes)>());
      return schema::Type{schema::Type::AnyOf{.types_ = std::move(types)}};
    }
  }

 private:
  template <size_t _i>
  static void add_to_schema(std::map<std::string, schema::Type>* _definitions,
                            std::vector<schema::Type>* _types) noexcept {
    using AltType =
        std::remove_cvref_t<internal::nth_element_t<_i, AlternativeTypes...>>;
    _types->push_back(
        Parser<R, W, AltType, ProcessorsType>::to_schema(_definitions));
  }

  template <int... _is>
  static void build_schema(std::map<std::string, schema::Type>* _definitions,
                           std::vector<schema::Type>* _types,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_to_schema<_is>(_definitions, _types), ...);
  }

  template <int _i>
  static void read_one_alternative(
      const R& _r, const InputVarType& _var,
      std::optional<rfl::Variant<AlternativeTypes...>>* _result,
      std::vector<Error>* _errors) noexcept {
    if (!*_result) {
      using AltType =
          std::remove_cvref_t<internal::nth_element_t<_i, AlternativeTypes...>>;
      auto res = Parser<R, W, AltType, ProcessorsType>::read(_r, _var);
      if (res) {
        *_result = std::move(*res);
      } else {
        _errors->emplace_back(*res.error());
      }
    }
  }

  template <int... _is>
  static void read_variant(
      const R& _r, const InputVarType& _var,
      std::optional<rfl::Variant<AlternativeTypes...>>* _result,
      std::vector<Error>* _errors,
      std::integer_sequence<int, _is...>) noexcept {
    (read_one_alternative<_is>(_r, _var, _result, _errors), ...);
  }
};

}  // namespace rfl::parsing

#endif
