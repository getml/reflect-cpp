#ifndef RFL_PARSING_PARSER_VALIDATOR_HPP_
#define RFL_PARSING_PARSER_VALIDATOR_HPP_

#include <map>
#include <optional>
#include <type_traits>
#include <variant>

#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../Validator.hpp"
#include "../internal/add_tags_to_variants_v.hpp"
#include "../internal/all_fields.hpp"
#include "../internal/to_ptr_field.hpp"
#include "../visit.hpp"
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

template <class T>
struct is_validator : std::false_type {};

template <class T, class... Vs>
struct is_validator<Validator<T, Vs...>> : std::true_type {};

template <class T>
constexpr bool is_validator_v = is_validator<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserValidator {
  using InputVarType = typename R::InputVarType;

  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    using ValueType = std::remove_cvref_t<typename T::ValueType>;
    return Parser<R, W, ValueType, ProcessorsType>::read(_r, _var).and_then(
        [](auto&& _value) -> Result<T> {
          return T::from_value(std::move(_value));
        });
  }

  template <class P>
  static void write(const W& _w, const T& _val, const P& _parent) {
    using ValueType = std::remove_cvref_t<typename T::ValueType>;
    Parser<R, W, ValueType, ProcessorsType>::write(_w, _val.value(), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    using ValueType = std::remove_cvref_t<typename T::ValueType>;
    using ValidationType = std::remove_cvref_t<typename T::ValidationType>;
    return Type{Type::Validated{
        .type_ = Ref<Type>::make(
            Parser<R, W, ValueType, ProcessorsType>::to_schema(_definitions)),
        .validation_ = ValidationType::template to_schema<ValueType>()}};
  }
};

}  // namespace rfl::parsing

#endif
