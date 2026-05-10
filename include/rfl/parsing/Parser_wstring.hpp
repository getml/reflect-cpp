#ifndef RFL_PARSING_PARSER_WSTRING_HPP_
#define RFL_PARSING_PARSER_WSTRING_HPP_

#include <map>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::wstring>
struct Parser<R, W, std::wstring, ProcessorsType> {
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
    // if (auto err = inStr.error(); err.has_value()) {
    //   return Result<std::wstring>(err.value());
    // }

    std::mbstate_t state = std::mbstate_t();
    auto val = inStr.value();

    std::wstring outStr(val.size() * 2, L'\0');

    // Explicitly set the size so we don't empty it when we truncate
    outStr.resize(val.size() * 2);

    auto* ptr = val.c_str();

    // Add 1 for null terminator
    auto len = std::mbsrtowcs(outStr.data(), &ptr, val.size(), &state);
    outStr.resize(len);  // Truncate the extra bytes

    return Result<std::wstring>(outStr);
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
    if (_str.empty()) {
      ParentType::add_value(_w, std::string(), _parent);
      return;
    }

    std::mbstate_t state = std::mbstate_t();
    std::string outStr(_str.size(), '\0');
    outStr.resize(_str.size());

    auto* ptr = _str.c_str();
    auto len = std::wcsrtombs(outStr.data(), &ptr, _str.size(), &state);
    outStr.resize(len);

    ParentType::add_value(_w, outStr, _parent);
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

}  // namespace parsing
}  // namespace rfl

#endif
