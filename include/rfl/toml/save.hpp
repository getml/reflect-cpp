#ifndef RFL_TOML_SAVE_HPP_
#define RFL_TOML_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace toml {

/// Saves an object to a TOML file.
/// Serializes a C++ object to TOML and writes it to a file using compile-time reflection.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the TOML file will be saved
/// @param _obj The object to serialize to TOML
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace toml
}  // namespace rfl

#endif
