#ifndef RFL_PARSING_PARSER_RENAME_HPP_
#define RFL_PARSING_PARSER_RENAME_HPP_

#include <map>
#include <type_traits>

#include "../Rename.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/StringLiteral.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, internal::StringLiteral _name,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, Rename<_name, T>>
struct Parser<R, W, Rename<_name, T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a renamed value from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed value or an error.
   */
  static Result<Rename<_name, T>> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    const auto to_rename = [](auto&& _t) {
      return Rename<_name, T>(std::move(_t));
    };
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform(to_rename);
  }

  /**
   * @brief Writes a renamed value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _rename The renamed value to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Rename<_name, T>& _rename,
                    const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _rename.value(), _parent);
  }

  /**
   * @brief Generates the schema for the renamed value.
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

}  // namespace parsing
}  // namespace rfl

#endif
