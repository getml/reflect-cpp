#ifndef RFL_TOML_READ_HPP_
#define RFL_TOML_READ_HPP_

#include <istream>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

#include "../Processors.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::toml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a TOML var (internal TOML node representation).
/// A TOML var is the internal representation used by the toml++ library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _var The TOML variant object to parse from
/// @return The parsed object of type T
template <class T, class... Ps>
auto read(InputVarType _var) {
  const auto r = Reader();
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  return Parser<T, ProcessorsType>::read(r, _var);
}

/// Reads a TOML string and parses it into an object.
/// This function reads a TOML string and constructs a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _toml_str The TOML string to parse
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const std::string_view _toml_str) noexcept {
#if TOML_EXCEPTIONS
  try {
    auto table = ::toml::parse(_toml_str);
    return read<T, Ps...>(&table);
  } catch (const std::exception& e) {
    return error(e.what());
  }
#else
  auto result = ::toml::parse(_toml_str);
  if (!result) {
    return error(std::string(result.error().description()));
  }
  return read<T, Ps...>(&result.table());
#endif
}

/// Parses an object from a stringstream containing TOML.
/// Reads the entire stream content and parses it as TOML.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing TOML data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto toml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T, Ps...>(toml_str);
}

}  // namespace rfl::toml

#endif
