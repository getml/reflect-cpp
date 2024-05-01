#ifndef RFL_TOML_READ_HPP_
#define RFL_TOML_READ_HPP_

#include <istream>
#include <string>
#include <toml++/toml.hpp>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::toml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a TOML var.
template <class T, class... Ps>
auto read(InputVarType _var) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _var);
}

/// Parses an object from TOML using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::string& _toml_str) {
  auto table = ::toml::parse(_toml_str);
  return read<T, Ps...>(&table);
}

/// Parses an object from a stringstream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto toml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T, Ps...>(toml_str);
}

}  // namespace rfl::toml

#endif
