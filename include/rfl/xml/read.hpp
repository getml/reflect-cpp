#ifndef RFL_XML_READ_HPP_
#define RFL_XML_READ_HPP_

#include <istream>
#include <pugixml.hpp>
#include <string>
#include <string_view>

#include "../Processors.hpp"
#include "../internal/get_type_name.hpp"
#include "../internal/remove_namespaces.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace xml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from a XML var.
template <class T, class... Ps>
auto read(const InputVarType& _var) {
  const auto r = Reader();
  using ProcessorsType = Processors<Ps...>;
  static_assert(!ProcessorsType::no_field_names_,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  return Parser<T, ProcessorsType>::read(r, _var);
}

/// Parses an object from XML using reflection.
template <class T, class... Ps>
Result<T> read(const std::string_view _xml_str) {
  pugi::xml_document doc;
  const auto result = doc.load_string(_xml_str.data());
  if (!result) {
    return error("XML string could not be parsed: " +
                 std::string(result.description()));
  }
  const auto var = InputVarType(doc.first_child());
  return read<T, Ps...>(var);
}

/// Parses an object from a stringstream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto xml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                   std::istreambuf_iterator<char>());
  return read<T, Ps...>(xml_str);
}

}  // namespace xml
}  // namespace rfl

#endif
