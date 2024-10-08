#ifndef RFL_XML_WRITE_HPP_
#define RFL_XML_WRITE_HPP_

#include <ostream>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../internal/StringLiteral.hpp"
#include "../internal/get_type_name.hpp"
#include "../internal/remove_namespaces.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace xml {

template <internal::StringLiteral _root, class T>
consteval auto get_root_name() {
  if constexpr (_root != internal::StringLiteral("")) {
    return _root;
  } else {
    return internal::remove_namespaces<
        internal::get_type_name<std::remove_cvref_t<T>>()>();
  }
}

/// Writes a XML into an ostream.
template <internal::StringLiteral _root = internal::StringLiteral(""),
          class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream,
                    const std::string& _indent = "    ") {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;

  constexpr auto root_name = get_root_name<_root, T>();

  static_assert(root_name.string_view().find("<") == std::string_view::npos &&
                    root_name.string_view().find(">") == std::string_view::npos,
                "The name of an XML root node cannot contain '<' or '>'. "
                "Please assign an "
                "explicit root name to rfl::xml::write(...) like this: "
                "rfl::xml::write<\"root_name\">(...).");

  const auto doc = rfl::Ref<pugi::xml_document>::make();

  auto declaration_node = doc->append_child(pugi::node_declaration);
  declaration_node.append_attribute("version") = "1.0";
  declaration_node.append_attribute("encoding") = "UTF-8";

  auto w = Writer(doc, root_name.str());

  using ProcessorsType = Processors<Ps...>;
  static_assert(!ProcessorsType::no_field_names_,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(w, _obj, typename ParentType::Root{});

  doc->save(_stream, _indent.c_str());

  return _stream;
}

/// Returns a XML string.
template <internal::StringLiteral _root = internal::StringLiteral(""),
          class... Ps>
std::string write(const auto& _obj, const std::string& _indent = "    ") {
  std::stringstream stream;
  write<_root, Ps...>(_obj, stream);
  return stream.str();
}

}  // namespace xml
}  // namespace rfl

#endif
