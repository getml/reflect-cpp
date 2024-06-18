#ifndef RFL_YAML_WRITE_HPP_
#define RFL_YAML_WRITE_HPP_

#include <yaml-cpp/yaml.h>

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace yaml {

/// Writes a YAML into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!ProcessorsType::no_field_names_,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  _stream << out->c_str();
  return _stream;
}

/// Returns a YAML string.
template <class... Ps>
std::string write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!ProcessorsType::no_field_names_,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});
  return out->c_str();
}

}  // namespace yaml
}  // namespace rfl

#endif
