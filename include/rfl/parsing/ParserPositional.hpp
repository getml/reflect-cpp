#ifndef RFL_PARSING_PARSER_POSITIONAL_HPP_
#define RFL_PARSING_PARSER_POSITIONAL_HPP_

#include <map>
#include <type_traits>

#include "../Positional.hpp"
#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_positional : std::false_type {};

template <class T>
struct is_positional<Positional<T>> : std::true_type {};

template <class T>
inline constexpr bool is_positional_v =
    is_positional<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserPositional {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a Positional value from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Positional value or an error.
   */
  static Result<Positional<T>> read(const R& _r,
                                    const InputVarType& _var) noexcept {
    const auto to_positional = [](auto&& _t) {
      return Positional<T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_positional);
  }

  /**
   * @brief Writes a Positional value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _positional The Positional value to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Positional<T>& _positional,
                    const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _positional.value(), _parent);
  }

  /**
   * @brief Generates the schema for the Positional value.
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
