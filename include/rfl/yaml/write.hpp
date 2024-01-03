#ifndef RFL_YAML_WRITE_HPP_
#define RFL_YAML_WRITE_HPP_

#include <yaml-cpp/yaml.h>

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace yaml {

/// Writes a XML into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) {
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  _stream << out->c_str();
  return _stream;
}

/// Returns a XML string.
template <class T>
std::string write(const T& _obj) {
  using ParentType = parsing::Parent<Writer>;
  const auto out = Ref<YAML::Emitter>::make();
  auto w = Writer(out);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  return out->c_str();
}

}  // namespace yaml
}  // namespace rfl

#endif  // XML_PARSER_HPP_
