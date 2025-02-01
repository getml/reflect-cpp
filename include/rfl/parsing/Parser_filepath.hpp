#ifndef RFL_PARSING_PARSER_FILEPATH_HPP_
#define RFL_PARSING_PARSER_FILEPATH_HPP_

#include <filesystem>
#include <utility>

#include "../Result.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::filesystem::path>
struct Parser<R, W, std::filesystem::path, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /// Expresses the variables as type T.
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

  template <class P>
  static void write(const W& _w, const std::filesystem::path& _p,
                    const P& _parent) noexcept {
    return Parser<R, W, std::string, ProcessorsType>::write(_w, _p.string(),
                                                            _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::string, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
