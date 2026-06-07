#ifndef RFL_PARSING_PARSER_RESULT_HPP_
#define RFL_PARSING_PARSER_RESULT_HPP_

#include <map>
#include <type_traits>

#include "../Field.hpp"
#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../internal/StringLiteral.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_result : std::false_type {};

template <class T>
struct is_result<Result<T>> : std::true_type {};

template <class T>
constexpr bool is_result_v = is_result<std::remove_cvref_t<T>>::value;

template <class R, class W, class ResultType, class ProcessorsType>
struct ParserResult {
  using T = typename ResultType::value_type;

  using InputVarType = typename R::InputVarType;

  using ErrorType = NamedTuple<Field<"error", std::string>>;
  using VariantType = std::variant<std::remove_cvref_t<T>, ErrorType>;

  /**
   * @brief Reads a Result from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Result or an error.
   */
  static Result<Result<T>> read(const R& _r,
                                const InputVarType& _var) noexcept {
    const auto handle = [](auto&& _t) -> Result<T> {
      using Type = std::remove_cvref_t<decltype(_t)>;
      if constexpr (std::is_same<Type, ErrorType>()) {
        return error(_t.template get<"error">());
      } else {
        return std::forward<std::remove_cvref_t<T>>(_t);
      }
    };

    const auto to_res = [&](VariantType&& _v) -> Result<T> {
      return std::visit(handle, std::forward<VariantType>(_v));
    };

    return Result<Result<T>>(
        Parser<R, W, VariantType, ProcessorsType>::read(_r, _var).transform(
            to_res));
  }

  /**
   * @brief Writes a Result to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _r The Result to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Result<T>& _r, const P& _parent) {
    if (_r) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
          _w, _r.value(), _parent);
    } else {
      Parser<R, W, ErrorType, ProcessorsType>::write(
          _w, ErrorType(make_field<"error">(_r.error().what())), _parent);
    }
  }

  /**
   * @brief Generates the schema for the Result.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace rfl::parsing

#endif
