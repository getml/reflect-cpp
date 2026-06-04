#ifndef RFL_PARSING_PARSER_COMMENTED_HPP_
#define RFL_PARSING_PARSER_COMMENTED_HPP_

#include <map>
#include <type_traits>

#include "../Commented.hpp"
#include "../Result.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"
#include "supports_comments.hpp"

namespace rfl::parsing {

template <class T>
struct is_commented : std::false_type {};

template <class T>
struct is_commented<Commented<T>> : std::true_type {};

template <class T>
constexpr bool is_commented_v = is_commented<std::remove_cvref_t<T>>::value;

template <class R, class W, class CommentedType, class ProcessorsType>
struct ParserCommented {
  using T = typename CommentedType::Type;

  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads a Commented value from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed Commented value or an error.
   */
  static Result<Commented<T>> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r, _var)
        .transform([](auto&& _t) {
          return Commented<T>(std::forward<decltype(_t)>(_t));
        });
  }

  /**
   * @brief Writes a Commented value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _c The Commented value to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const Commented<T>& _c, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _c.get(),
                                                                _parent);
    if constexpr (supports_comments<W>) {
      if (_c.comment()) {
        ParentType::add_comment(_w, *_c.comment(), _parent);
      }
    }
  }

  /**
   * @brief Generates the schema for the Commented value.
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
