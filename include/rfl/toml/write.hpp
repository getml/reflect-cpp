#ifndef RFL_TOML_WRITE_HPP_
#define RFL_TOML_WRITE_HPP_

#include <ostream>
#include <sstream>
#include <string>
#include <toml++/toml.hpp>
#include <type_traits>

#include "../Processors.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::toml {

/// Writes a TOML representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to TOML and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to TOML
/// @param _stream The output stream to write TOML to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  ::toml::table root;
  auto w = Writer(&root);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  _stream << root;
  return _stream;
}

/// Returns a TOML string representation of the object.
/// Uses compile-time reflection to serialize a C++ object to TOML format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to TOML
/// @return TOML string representation of the object
template <class... Ps>
std::string write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  std::stringstream sstream;
  ::toml::table root;
  auto w = Writer(&root);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  sstream << root;
  return sstream.str();
}

}  // namespace rfl::toml

#endif
