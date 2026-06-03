#ifndef RFL_PARSING_PARSER_STRING_VIEW_HPP_
#define RFL_PARSING_PARSER_STRING_VIEW_HPP_

#include <cstring>
#include <map>
#include <string>
#include <string_view>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/allow_raw_ptrs_v.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_string_view : std::false_type {};

template <>
struct is_string_view<std::string_view> : std::true_type {};

template <class T>
constexpr bool is_string_view_v = is_string_view<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserStringView {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a string_view from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed string_view or an error.
   */
  static Result<std::string_view> read(const R& _r,
                                       const InputVarType& _var) noexcept {
    if constexpr (!internal::allow_raw_ptrs_v<ProcessorsType>) {
      static_assert(
          always_false_v<R>,
          "Reading into std::string_view is dangerous and "
          "therefore unsupported. "
          "Please consider using std::string instead or wrapping "
          "std::string in rfl::Box or rfl::Ref."
          "If you absolutely must use std::string_view, "
          "you can pass the rfl::AllowRawPtrs processor. "
          "Please note that it is then YOUR responsibility "
          "to delete the allocated memory. Please also refer "
          "to the related documentation (in the section on processors).");
      return error("Unsupported.");
    } else {
      return Parser<R, W, std::string, ProcessorsType>::read(_r, _var)
          .transform([](std::string&& str) {
            char* data =
                new char[str.size() + 1];  // +1 for the null terminator
            std::memcpy(data, str.data(), str.size() + 1);
            return std::string_view(data, str.size());
          });
    }
  }

  /**
   * @brief Writes a string_view to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _str The string_view to write.
   * @param _p The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::string_view& _str, const P& _p) {
    Parser<R, W, std::string, ProcessorsType>::write(_w, std::string(_str), _p);
  }

  /**
   * @brief Generates the schema for the string_view.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::string, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
