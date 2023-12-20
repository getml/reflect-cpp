#ifndef RFL_XML_WRITE_HPP_
#define RFL_XML_WRITE_HPP_

#include <ostream>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <type_traits>

#include "rfl/internal/StringLiteral.hpp"
#include "rfl/internal/get_type_name.hpp"
#include "rfl/internal/remove_namespaces.hpp"
#include "rfl/xml/Parser.hpp"

namespace rfl {
namespace xml {

template <internal::StringLiteral _root, class T>
consteval auto get_root_name() {
  if constexpr (_root != internal::StringLiteral("")) {
    return _root;
  } else {
    return internal::remove_namespaces<
        internal::get_type_name<std::decay_t<T>>()>();
  }
}

/// Writes a XML into an ostream.
template <internal::StringLiteral _root = internal::StringLiteral(""), class T>
std::ostream& write(const T& _obj, std::ostream& _stream,
                    const std::string& _indent = "    ") {
  constexpr auto root_name = get_root_name<_root, T>();

  static_assert(root_name.string_view().find("<") == std::string_view::npos &&
                    root_name.string_view().find(">") == std::string_view::npos,
                "The name of an XML root node cannot contain '<' or '>'. "
                "Please assign an "
                "explicit root name to rfl::xml::write(...) like this: "
                "rfl::xml::write<\"root_name\">(...).");

  auto w = Writer();
  const auto xml_obj = Parser<T>::write(w, _obj);

  pugi::xml_document doc;

  auto declaration_node = doc.append_child(pugi::node_declaration);
  declaration_node.append_attribute("version") = "1.0";
  declaration_node.append_attribute("encoding") = "UTF-8";

  xml_obj->insert(root_name.str(), &doc);

  doc.save(_stream, _indent.c_str());

  return _stream;
}

/// Returns a XML string.
template <internal::StringLiteral _root = internal::StringLiteral(""), class T>
std::string write(const T& _obj, const std::string& _indent = "    ") {
  std::stringstream stream;
  write<_root, T>(_obj, stream);
  return stream.str();
}

}  // namespace xml
}  // namespace rfl

#endif  // XML_PARSER_HPP_
