#ifndef RFL_PARSING_PARSER_VARIANT_HPP_
#define RFL_PARSING_PARSER_VARIANT_HPP_

#include <map>
#include <optional>
#include <type_traits>
#include <variant>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"
#include "../internal/to_ptr_field.hpp"
#include "FieldVariantParser.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "to_single_error_message.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... FieldTypes, class ProcessorsType>
requires AreReaderAndWriter<R, W, std::variant<FieldTypes...>>
class Parser<R, W, std::variant<FieldTypes...>, ProcessorsType> {
  template <class T>
  using ptr_field_t =
      decltype(internal::to_ptr_field(std::declval<const T&>()));

 public:
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<std::variant<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    if constexpr (internal::all_fields<std::tuple<FieldTypes...>>()) {
      const auto wrap = [](auto&& _v) {
        return std::variant<FieldTypes...>(std::move(_v));
      };
      const auto to_std_variant = [&](auto&& _v) {
        return rfl::visit(wrap, std::move(_v));
      };
      return FieldVariantParser<R, W, ProcessorsType, FieldTypes...>::read(_r,
                                                                           _var)
          .transform(to_std_variant);
    } else {
      std::optional<std::variant<FieldTypes...>> result;
      std::vector<Error> errors;
      read_variant(_r, _var, &result, &errors);
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
  static void write(const W& _w, const std::variant<FieldTypes...>& _variant,
                    const P& _parent) noexcept {
    if constexpr (internal::all_fields<std::tuple<FieldTypes...>>()) {
      const auto wrap = [](const auto& _v) {
        return rfl::Variant<ptr_field_t<FieldTypes>...>(
            internal::to_ptr_field(_v));
      };
      const auto to_rfl_variant = [&](const auto& _v) {
        return std::visit(wrap, _v);
      };
      FieldVariantParser<R, W, ProcessorsType,
                         ptr_field_t<FieldTypes>...>::write(_w,
                                                            to_rfl_variant(
                                                                _variant),
                                                            _parent);
    } else {
      const auto handle = [&](const auto& _v) {
        using Type = std::remove_cvref_t<decltype(_v)>;
        Parser<R, W, Type, ProcessorsType>::write(_w, _v, _parent);
      };
      return std::visit(handle, _variant);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    if constexpr (internal::all_fields<std::tuple<FieldTypes...>>()) {
      return FieldVariantParser<R, W, ProcessorsType, FieldTypes...>::to_schema(
          _definitions);
    } else {
      std::vector<schema::Type> types;
      build_schema(_definitions, &types,
                   std::make_integer_sequence<int, sizeof...(FieldTypes)>());
      return schema::Type{schema::Type::AnyOf{.types_ = std::move(types)}};
    }
  }

 private:
  template <size_t _i>
  static void add_to_schema(std::map<std::string, schema::Type>* _definitions,
                            std::vector<schema::Type>* _types) noexcept {
    using U = std::remove_cvref_t<
        std::variant_alternative_t<_i, std::variant<FieldTypes...>>>;
    _types->push_back(Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
  }

  template <int... _is>
  static void build_schema(std::map<std::string, schema::Type>* _definitions,
                           std::vector<schema::Type>* _types,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_to_schema<_is>(_definitions, _types), ...);
  }

  template <int _i = 0>
  static void read_variant(const R& _r, const InputVarType& _var,
                           std::optional<std::variant<FieldTypes...>>* _result,
                           std::vector<Error>* _errors) noexcept {
    constexpr size_t size = sizeof...(FieldTypes);
    if constexpr (_i < size) {
      using AltType = std::remove_cvref_t<
          std::variant_alternative_t<_i, std::variant<FieldTypes...>>>;
      auto res = Parser<R, W, AltType, ProcessorsType>::read(_r, _var);
      if (res) {
        *_result = std::move(*res);
        return;
      } else {
        _errors->emplace_back(*res.error());
        return read_variant<_i + 1>(_r, _var, _result, _errors);
      }
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
