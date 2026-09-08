#ifndef RFL_YAML_READ_HPP_
#define RFL_YAML_READ_HPP_

#include <yaml-cpp/yaml.h>

#include <istream>
#include <string>
#include <string_view>

#include "../Processors.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace yaml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a YAML var (internal YAML node representation).
/// A YAML var is the internal representation used by the yaml-cpp library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _var The YAML variant object to parse from
/// @param _yaml_str The original YAML string (used for error context)
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(const InputVarType& _var, const std::string& _yaml_str) {
  const auto r = Reader(_yaml_str);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  return Parser<T, ProcessorsType>::read(r, _var);
}

/// Parses an object from YAML using reflection.
/// This function reads a YAML string and constructs a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _yaml_str The YAML string to parse
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::string& _yaml_str) {
  try {
    const auto var = InputVarType(YAML::Load(_yaml_str));
    return read<T, Ps...>(var, _yaml_str);
  } catch (std::exception& e) {
    return error(e.what());
  }
}

/// Parses an object from YAML using reflection (string_view version).
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _yaml_str The YAML string to parse
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const std::string_view _yaml_str) {
  return read<T, Ps...>(std::string(_yaml_str));
}

/// Parses an object from a stringstream containing YAML.
/// Reads the entire stream content and parses it as YAML.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing YAML data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto yaml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T, Ps...>(yaml_str);
}

}  // namespace yaml
}  // namespace rfl

#endif
