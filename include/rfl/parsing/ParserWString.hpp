#ifndef RFL_PARSING_PARSER_WSTRING_HPP_
#define RFL_PARSING_PARSER_WSTRING_HPP_

#include <map>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/strings/utf8_conversions.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_wstring : std::false_type {};

template <>
struct is_wstring<std::wstring> : std::true_type {};

template <class T>
constexpr bool is_wstring_v = is_wstring<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserWString {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads a wstring from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed wstring or an error.
   */
  static Result<std::wstring> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    if (_r.is_empty(_var)) {
      return std::wstring();
    }

    auto inStr = Parser<R, W, std::string, ProcessorsType>::read(_r, _var);
    if (!inStr) {
      return Result<std::wstring>(error(inStr.error()));
    }

    // JSON strings are UTF-8 (RFC 8259). Convert explicitly rather than via the
    // locale-dependent std::mbsrtowcs, which returns (size_t)-1 on any input
    // that is not valid in the current C locale's encoding.
    auto outStr = internal::strings::utf8_to_wstring(inStr.value());
    if (!outStr) {
      return Result<std::wstring>(
          error("Could not parse the string: it is not valid UTF-8."));
    }

    return Result<std::wstring>(std::move(*outStr));
  }

  /**
   * @brief Writes a wstring to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _str The wstring to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::wstring& _str, const P& _parent) {
    // Emit the wstring as UTF-8 (RFC 8259). wstring_to_utf8 only fails on
    // genuinely malformed input (e.g. unpaired surrogates); Parser<...>::write
    // returns void and the Writer has no error channel, so emit an empty string
    // in that case rather than crash.
    const auto outStr = internal::strings::wstring_to_utf8(_str);
    ParentType::add_value(_w, outStr.value_or(std::string()), _parent);
  }

  /**
   * @brief Generates the schema for the wstring.
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
