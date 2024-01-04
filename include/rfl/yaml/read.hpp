#ifndef RFL_YAML_READ_HPP_
#define RFL_YAML_READ_HPP_

#include <yaml-cpp/yaml.h>

#include <istream>
#include <string>

#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace yaml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a YAML var.
template <class T>
auto read(const InputVarType& _var) {
  const auto r = Reader();
  return Parser<T>::read(r, _var);
}

/// Parses an object from YAML using reflection.
template <class T>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::string& _yaml_str) {
  try {
    const auto var = InputVarType(YAML::Load(_yaml_str));
    return read<T>(var);
  } catch (std::exception& e) {
    return Error(e.what());
  }
}

/// Parses an object from a stringstream.
template <class T>
auto read(std::istream& _stream) {
  const auto yaml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T>(yaml_str);
}

}  // namespace yaml
}  // namespace rfl

#endif
