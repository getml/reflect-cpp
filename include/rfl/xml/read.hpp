#ifndef RFL_XML_READ_HPP_
#define RFL_XML_READ_HPP_

#include <istream>
#include <pugixml.hpp>
#include <string>
#include <string_view>

#include "../Processors.hpp"
#include "../internal/get_type_name.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/remove_namespaces.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl ::xml {

using InputVarType = typename Reader::InputVarType;

/// Parses an object from an XML var (internal XML node representation).
/// An XML var is the internal representation used by the pugixml library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _var The XML variant object to parse from
/// @return Result containing the parsed object of type T
template <class T, class... Ps>
auto read(const InputVarType& _var) {
  const auto r = Reader();
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  return Parser<T, ProcessorsType>::read(r, _var);
}

/// Parses an object from XML using reflection.
/// This function reads an XML string and constructs a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _xml_str The XML string to parse
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
Result<T> read(const std::string_view _xml_str) {
  pugi::xml_document doc;
  const auto result = doc.load_buffer(_xml_str.data(), _xml_str.size());
  if (!result) {
    return error("XML string could not be parsed: " +
                 std::string(result.description()));
  }
  const auto var = InputVarType(doc.first_child());
  return read<T, Ps...>(var);
}

/// Parses an object from a stringstream containing XML.
/// Reads the entire stream content and parses it as XML.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing XML data
/// @return Result containing either the parsed object of type T or an error message
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto xml_str = std::string(std::istreambuf_iterator<char>(_stream),
                                   std::istreambuf_iterator<char>());
  return read<T, Ps...>(xml_str);
}

}  // namespace rfl::xml

#endif
