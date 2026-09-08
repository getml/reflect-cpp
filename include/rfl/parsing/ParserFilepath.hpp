#ifndef RFL_PARSING_PARSER_FILEPATH_HPP_
#define RFL_PARSING_PARSER_FILEPATH_HPP_

#include <filesystem>
#include <map>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_filepath : std::false_type {};

template <>
struct is_filepath<std::filesystem::path> : std::true_type {};

template <class T>
constexpr bool is_filepath_v = is_filepath<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserFilepath {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Reads a std::filesystem::path from the input.
   * @param _r The reader to use.
   * @param _var The input variable to read.
   * @return A Result containing the parsed path or an error.
   */
  static Result<std::filesystem::path> read(const R& _r,
                                            const InputVarType& _var) noexcept {
    const auto to_path =
        [&](std::string&& _str) -> Result<std::filesystem::path> {
      try {
        // JSON strings are UTF-8 (RFC 8259); construct the path from UTF-8
        // explicitly rather than via the locale-dependent narrow constructor.
        return std::filesystem::path(std::u8string(
            reinterpret_cast<const char8_t*>(_str.data()), _str.size()));
      } catch (std::exception& e) {
        return error(e.what());
      }
    };
    return Parser<R, W, std::string, ProcessorsType>::read(_r, _var).and_then(
        to_path);
  }

  /**
   * @brief Writes a std::filesystem::path to the output.
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _p The path to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::filesystem::path& _p,
                    const P& _parent) {
    // Emit the path as UTF-8 (RFC 8259); _p.string() is locale-dependent and
    // throws / mangles on Windows for non-representable characters.
    const auto u8 = _p.u8string();
    return Parser<R, W, std::string, ProcessorsType>::write(
        _w, std::string(reinterpret_cast<const char*>(u8.c_str()), u8.size()),
        _parent);
  }

  /**
   * @brief Generates the schema for std::filesystem::path.
   * @param _definitions The map of definitions to add to.
   * @return The schema type for std::filesystem::path.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::string, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
