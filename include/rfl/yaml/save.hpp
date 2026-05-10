#ifndef RFL_YAML_SAVE_HPP_
#define RFL_YAML_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../io/save_string.hpp"
#include "write.hpp"

namespace rfl {
namespace yaml {

/// Saves an object to a YAML file.
/// Serializes a C++ object to YAML and writes it to a file using compile-time reflection.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _fname The filename/path where the YAML file will be saved
/// @param _obj The object to serialize to YAML
/// @return Result containing Nothing on success or an error message on failure
template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj, auto& _stream) -> auto& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_string(_fname, _obj, write_func);
}

}  // namespace yaml
}  // namespace rfl

#endif
