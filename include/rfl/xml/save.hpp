#ifndef RFL_XML_SAVE_HPP_
#define RFL_XML_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../internal/StringLiteral.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace xml {

/// Saves an object to an XML file.
/// Serializes a C++ object to XML and writes it to a file using compile-time reflection.
/// @tparam _root The name of the XML root element (defaults to type name if empty)
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the XML file will be saved
/// @param _obj The object to serialize to XML
/// @return Result containing Nothing on success or an error message on failure
template <internal::StringLiteral _root = internal::StringLiteral(""),
          class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<_root, Ps...>(_obj, _stream);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace xml
}  // namespace rfl

#endif
