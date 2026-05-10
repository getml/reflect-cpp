#ifndef RFL_PARSING_PARSER_FILEPATH_HPP_
#define RFL_PARSING_PARSER_FILEPATH_HPP_

#include <filesystem>
#include <map>

#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::filesystem::path>
struct Parser<R, W, std::filesystem::path, ProcessorsType> {
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
        return std::filesystem::path(_str);
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
    return Parser<R, W, std::string, ProcessorsType>::write(_w, _p.string(),
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

}  // namespace parsing
}  // namespace rfl

#endif
