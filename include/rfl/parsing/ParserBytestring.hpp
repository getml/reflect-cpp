#ifndef RFL_PARSING_PARSER_BYTESTRING_HPP_
#define RFL_PARSING_PARSER_BYTESTRING_HPP_

#include <map>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_bytestring : std::false_type {};

template <>
struct is_bytestring<Bytestring> : std::true_type {};

template <class T>
constexpr bool is_bytestring_v = is_bytestring<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserBytestring {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  /**
   * @brief Reads a Bytestring from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Bytestring or an error.
   */
  static Result<Bytestring> read(const R& _r,
                                 const InputVarType& _var) noexcept {
    return _r.template to_basic_type<Bytestring>(_var);
  }

  /**
   * @brief Writes a Bytestring to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _b The Bytestring to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Bytestring& _b, const P& _parent) {
    ParentType::add_value(_w, _b, _parent);
  }

  /**
   * @brief Generates the schema for the Bytestring.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(std::map<std::string, schema::Type>*) {
    return schema::Type{schema::Type::Bytestring{}};
  }
};

}  // namespace rfl::parsing

#endif
