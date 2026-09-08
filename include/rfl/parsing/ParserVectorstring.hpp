#ifndef RFL_PARSING_PARSER_VECTORSRING_HPP_
#define RFL_PARSING_PARSER_VECTORSRING_HPP_

#include <map>

#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_vectorstring : std::false_type {};

template <>
struct is_vectorstring<Vectorstring> : std::true_type {};

template <class T>
constexpr bool is_vectorstring_v =
    is_vectorstring<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserVectorstring {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  /**
   * @brief Reads a Vectorstring from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Vectorstring or an error.
   */
  static Result<Vectorstring> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    return _r.template to_basic_type<Vectorstring>(_var);
  }

  /**
   * @brief Writes a Vectorstring to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _b The Vectorstring to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Vectorstring& _b, const P& _parent) {
    ParentType::add_value(_w, _b, _parent);
  }

  /**
   * @brief Generates the schema for the Vectorstring.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(std::map<std::string, schema::Type>*) {
    return schema::Type{schema::Type::Vectorstring{}};
  }
};

}  // namespace rfl::parsing

#endif
