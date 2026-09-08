#ifndef RFL_PARSING_PARSER_SHORT_HPP_
#define RFL_PARSING_PARSER_SHORT_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../Short.hpp"
#include "../internal/StringLiteral.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_short : std::false_type {};

template <class T, internal::StringLiteral _name>
struct is_short<Short<_name, T>> : std::true_type {
  using element_type = T;
  static constexpr auto name = _name;
};

template <class T>
constexpr bool is_short_v = is_short<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserShort;

template <class R, class W, class T, internal::StringLiteral _name,
          class ProcessorsType>
struct ParserShort<R, W, Short<_name, T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a short value from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed value or an error.
   */
  static Result<Short<_name, T>> read(const R& _r,
                                      const InputVarType& _var) noexcept {
    const auto to_short = [](auto&& _t) {
      return Short<_name, T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_short);
  }

  /**
   * @brief Writes a short value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _short The short value to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Short<_name, T>& _short,
                    const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _short.value(), _parent);
  }

  /**
   * @brief Generates the schema for the short value.
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
