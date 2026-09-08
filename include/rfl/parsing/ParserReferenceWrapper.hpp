#ifndef RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_
#define RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_

#include <functional>
#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_reference_wrapper : std::false_type {};

template <class T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v =
    is_reference_wrapper<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserReferenceWrapper {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reading into a reference wrapper is not supported.
   *
   * @return An error.
   */
  static Result<std::reference_wrapper<T>> read(const R&,
                                                const InputVarType&) noexcept {
    static_assert(always_false_v<T>,
                  "Reading into std::reference_wrapper is dangerous and "
                  "therefore unsupported. "
                  "Please consider using std::unique_ptr, rfl::Box, "
                  "std::shared_ptr, or "
                  "rfl::Ref instead.");
    return error("Unsupported.");
  }

  /**
   * @brief Writes a value from a reference wrapper to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _ref The reference wrapper to write.
   * @param _p The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::reference_wrapper<T> _ref,
                    const P& _p) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _ref.get(),
                                                                _p);
  }

  /**
   * @brief Generates the schema for the reference wrapper.
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
