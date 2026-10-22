#ifndef RFL_PARSING_PARSER_MONOSTATE_HPP_
#define RFL_PARSING_PARSER_MONOSTATE_HPP_

#include <map>
#include <string>
#include <type_traits>
#include <variant>

#include "../Result.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_monostate : std::false_type {};

template <>
struct is_monostate<std::monostate> : std::true_type {};

template <class T>
constexpr bool is_monostate_v = is_monostate<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserMonostate {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads a std::monostate from the input, which must be null.
   * @param _r The reader to use.
   * @param _var The input variable to read.
   * @return A Result containing the std::monostate or an error.
   */
  static Result<std::monostate> read(const R& _r,
                                     const InputVarType& _var) noexcept {
    if (!_r.is_empty(_var)) {
      return error("Expected null for std::monostate.");
    }
    return std::monostate{};
  }

  /**
   * @brief Writes a std::monostate to the output as null.
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::monostate&, const P& _parent) {
    ParentType::add_null(_w, _parent);
  }

  /**
   * @brief Generates the schema for std::monostate.
   * @return The null schema type.
   */
  static schema::Type to_schema(std::map<std::string, schema::Type>*) {
    return schema::Type{schema::Type::Null{}};
  }
};

}  // namespace rfl::parsing

#endif
