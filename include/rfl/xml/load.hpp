#ifndef RFL_XML_LOAD_HPP_
#define RFL_XML_LOAD_HPP_

#include "../Result.hpp"
#include "../io/load_string.hpp"
#include "read.hpp"

namespace rfl {
namespace xml {

/// Loads an object from an XML file.
/// Reads a file from disk and parses its XML content into a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _fname The filename/path of the XML file to load
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> load(const std::string& _fname) {
  const auto read_string = [](const auto& _str) {
    return read<T, Ps...>(_str);
  };
  return rfl::io::load_string(_fname).and_then(read_string);
}

}  // namespace xml
}  // namespace rfl

#endif
