#ifndef RFL_PARSING_PARSER_VARIANT_HPP_
#define RFL_PARSING_PARSER_VARIANT_HPP_

#include <map>
#include <optional>
#include <type_traits>
#include <variant>

#include "../NamedTuple.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"
#include "../internal/to_ptr_field.hpp"
#include "FieldVariantParser.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "VariantAlternativeWrapper.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"
#include "schemaful/VariantReader.hpp"
#include "to_single_error_message.hpp"

namespace rfl::parsing {

template <class R, class W, class... AlternativeTypes, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::variant<AlternativeTypes...>>
class Parser<R, W, std::variant<AlternativeTypes...>, ProcessorsType> {
  template <class T>
  using ptr_field_t =
      decltype(internal::to_ptr_field(std::declval<const T&>()));

  using ParentType = Parent<W>;

 public:
  using InputVarType = typename R::InputVarType;

  static Result<std::variant<AlternativeTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      if constexpr (schemaful::IsSchemafulReader<R>) {
        using WrappedType = rfl::Variant<NamedTuple<AlternativeTypes>...>;
        return Parser<R, W, WrappedType, ProcessorsType>::read(_r, _var)
            .transform(
                [](auto&& _variant) -> std::variant<AlternativeTypes...> {
                  return std::move(_variant).visit([](auto&& _named_tuple) {
                    return std::variant<AlternativeTypes...>(std::move(
                        std::move(_named_tuple).fields().template get<0>()));
                  });
                });

      } else {
        const auto wrap = [](auto&& _v) {
          return std::variant<AlternativeTypes...>(std::move(_v));
        };
        const auto to_std_variant = [&](auto&& _v) {
          return rfl::visit(wrap, std::move(_v));
        };
        return FieldVariantParser<R, W, ProcessorsType,
                                  AlternativeTypes...>::read(_r, _var)
            .transform(to_std_variant);
      }

    } else if constexpr (schemaful::IsSchemafulReader<R>) {
      using V =
          schemaful::VariantReader<R, W, std::variant<AlternativeTypes...>,
                                   ProcessorsType, AlternativeTypes...>;
      return _r.to_union(_var).and_then([&](const auto& _u) {
        return _r.template read_union<std::variant<AlternativeTypes...>, V>(_u);
      });

    } else if constexpr (ProcessorsType::add_tags_to_variants_) {
      using FieldVariantType =
          rfl::Variant<VariantAlternativeWrapper<AlternativeTypes>...>;
      const auto from_field_variant =
          [](auto&& _field) -> std::variant<AlternativeTypes...> {
        return std::move(_field.value());
      };
      return Parser<R, W, FieldVariantType, ProcessorsType>::read(_r, _var)
          .transform([&](FieldVariantType&& _f) {
            return _f.visit(from_field_variant);
          });

    } else {
      std::optional<std::variant<AlternativeTypes...>> result;
      std::vector<Error> errors;
      read_variant(
          _r, _var, &result, &errors,
          std::make_integer_sequence<int, sizeof...(AlternativeTypes)>());
      if (result) {
        return std::move(*result);
      } else {
        return error(
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
                    const std::variant<AlternativeTypes...>& _variant,
                    const P& _parent) noexcept {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      if constexpr (schemaful::IsSchemafulWriter<W>) {
        using WrappedType = rfl::Variant<
            NamedTuple<Field<AlternativeTypes::name_,
                             const typename AlternativeTypes::Type*>>...>;
        const auto to_wrapped = [](const auto& _variant) -> WrappedType {
          return std::visit(
              [](const auto& _field) -> WrappedType {
                return make_named_tuple(internal::to_ptr_field(_field));
              },
              _variant);
        };
        Parser<R, W, WrappedType, ProcessorsType>::write(
            _w, to_wrapped(_variant), _parent);

      } else {
        const auto wrap = [](const auto& _v) {
          return rfl::Variant<ptr_field_t<AlternativeTypes>...>(
              internal::to_ptr_field(_v));
        };
        const auto to_rfl_variant = [&](const auto& _v) {
          return std::visit(wrap, _v);
        };
        FieldVariantParser<
            R, W, ProcessorsType,
            ptr_field_t<AlternativeTypes>...>::write(_w,
                                                     to_rfl_variant(_variant),
                                                     _parent);
      }

    } else if constexpr (schemaful::IsSchemafulWriter<W>) {
      return std::visit(
          [&](const auto& _v) {
            using Type = std::remove_cvref_t<decltype(_v)>;
            auto u = ParentType::add_union(_w, _parent);
            using UnionType = typename ParentType::template Union<decltype(u)>;
            auto p = UnionType{.index_ = static_cast<size_t>(_variant.index()),
                               .union_ = &u};
            Parser<R, W, Type, ProcessorsType>::write(_w, _v, p);
          },
          _variant);

    } else if constexpr (ProcessorsType::add_tags_to_variants_) {
      using FieldVariantType =
          rfl::Variant<VariantAlternativeWrapper<const AlternativeTypes*>...>;
      const auto to_field_variant =
          []<class T>(const T& _t) -> FieldVariantType {
        return VariantAlternativeWrapper<const T*>(&_t);
      };
      Parser<R, W, FieldVariantType, ProcessorsType>::write(
          _w, std::visit(to_field_variant, _variant), _parent);

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
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
      return FieldVariantParser<R, W, ProcessorsType,
                                AlternativeTypes...>::to_schema(_definitions);

    } else if constexpr (ProcessorsType::add_tags_to_variants_) {
      using FieldVariantType =
          rfl::Variant<VariantAlternativeWrapper<AlternativeTypes>...>;
      return Parser<R, W, FieldVariantType, ProcessorsType>::to_schema(
          _definitions);

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
    using U = std::remove_cvref_t<
        std::variant_alternative_t<_i, std::variant<AlternativeTypes...>>>;
    _types->push_back(Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
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
      std::optional<std::variant<AlternativeTypes...>>* _result,
      std::vector<Error>* _errors) noexcept {
    if (!*_result) {
      using AltType =
          std::remove_cvref_t<internal::nth_element_t<_i, AlternativeTypes...>>;
      auto res = Parser<R, W, AltType, ProcessorsType>::read(_r, _var);
      if (res) {
        *_result = std::move(*res);
      } else {
        _errors->emplace_back(res.error());
      }
    }
  }

  template <int... _is>
  static void read_variant(
      const R& _r, const InputVarType& _var,
      std::optional<std::variant<AlternativeTypes...>>* _result,
      std::vector<Error>* _errors,
      std::integer_sequence<int, _is...>) noexcept {
    (read_one_alternative<_is>(_r, _var, _result, _errors), ...);
  }
};

}  // namespace rfl::parsing

#endif
