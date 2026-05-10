#ifndef RFL_YAML_WRITE_HPP_
#define RFL_YAML_WRITE_HPP_

#include <yaml-cpp/yaml.h>

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace yaml {

/// Writes a YAML representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to YAML and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to YAML
/// @param _stream The output stream to write YAML to
/// @param _flags Optional writer flags for formatting (default: no_flags)
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream, Writer::Flags _flags = Writer::Flags::no_flags) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out, _flags);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  _stream << out->c_str();
  return _stream;
}

/// Returns a YAML string representation of the object.
/// Uses compile-time reflection to serialize a C++ object to YAML format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to YAML
/// @param _flags Optional writer flags for formatting (default: no_flags)
/// @return YAML string representation of the object
template <class... Ps>
std::string write(const auto& _obj, Writer::Flags _flags = Writer::Flags::no_flags) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out, _flags);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  return out->c_str();
}

}  // namespace yaml
}  // namespace rfl

#endif
