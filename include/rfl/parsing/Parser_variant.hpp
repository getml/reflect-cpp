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
#include "../internal/add_tags_to_variants_v.hpp"
#include "../internal/all_fields.hpp"
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

  /**
   * @brief Reads a variant from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed variant or an error.
   */
  static Result<std::variant<AlternativeTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
--snip--
      }
    }
  }

  /**
   * @brief Writes a variant to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _variant The variant to write.
   * @param _parent The parent object to write into.
   */
  template <class P>
  static void write(const W& _w,
                    const std::variant<AlternativeTypes...>& _variant,
                    const P& _parent) {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
--snip--
      return std::visit(handle, _variant);
    }
  }

  /**
   * @brief Generates a schema for the variant.
   *
   * @param _definitions The map of definitions to add the schema to.
   * @return The generated schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    if constexpr (internal::all_fields<std::tuple<AlternativeTypes...>>()) {
--snip--
    }
  }

 private:
  /**
   * @brief Adds an alternative to the schema.
   *
   * @tparam _i The index of the alternative.
   * @param _definitions The map of definitions to add the schema to.
   * @param _types The vector of types to add the generated schema to.
   */
  template <size_t _i>
  static void add_to_schema(std::map<std::string, schema::Type>* _definitions,
                            std::vector<schema::Type>* _types) noexcept {
    using U = std::remove_cvref_t<
        std::variant_alternative_t<_i, std::variant<AlternativeTypes...>>>;
    _types->push_back(Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
  }

  /**
   * @brief Builds the schema for the variant.
   *
   * @tparam _is The indices of the alternatives.
   * @param _definitions The map of definitions to add the schema to.
   * @param _types The vector of types to add the generated schemas to.
   */
  template <int... _is>
  static void build_schema(std::map<std::string, schema::Type>* _definitions,
                           std::vector<schema::Type>* _types,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_to_schema<_is>(_definitions, _types), ...);
  }

  /**
   * @brief Reads a single alternative from the input.
   *
   * @tparam _i The index of the alternative.
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @param _result The result pointer to store the parsed alternative.
   * @param _errors The vector of errors to add parsing errors to.
   */
  template <int _i>
  static void read_one_alternative(
      const R& _r, const InputVarType& _var,
      std::optional<std::variant<AlternativeTypes...>>* _result,
      std::vector<std::string>* _errors) noexcept {
    if (!*_result) {
      using AltType =
          std::remove_cvref_t<internal::nth_element_t<_i, AlternativeTypes...>>;
      auto res = Parser<R, W, AltType, ProcessorsType>::read(_r, _var);
      if (res) {
        _result->emplace(std::move(*res));
      } else {
        _errors->emplace_back(res.error().what());
      }
    }
  }

  /**
   * @brief Reads the variant from the input by trying each alternative.
   *
   * @tparam _is The indices of the alternatives.
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @param _result The result pointer to store the parsed alternative.
   * @param _errors The vector of errors to add parsing errors to.
   */
  template <int... _is>
  static void read_variant(
      const R& _r, const InputVarType& _var,
      std::optional<std::variant<AlternativeTypes...>>* _result,
      std::vector<std::string>* _errors,
      std::integer_sequence<int, _is...>) noexcept {
    (read_one_alternative<_is>(_r, _var, _result, _errors), ...);
  }
};
};

}  // namespace rfl::parsing

#endif
