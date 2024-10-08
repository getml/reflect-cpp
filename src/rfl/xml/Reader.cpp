/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/xml/Reader.hpp"

namespace rfl::xml {

/// XML-only helper function. This is needed because XML distinguishes between
/// nodes and attributes.
static rfl::Result<pugi::xml_node> cast_as_node(
    const std::variant<pugi::xml_node, pugi::xml_attribute>&
        _node_or_attribute) {
  const auto cast = [](const auto& _n) -> Result<pugi::xml_node> {
    using Type = std::remove_cvref_t<decltype(_n)>;
    if constexpr (std::is_same<Type, pugi::xml_node>()) {
      return _n;
    } else {
      return Error("Field '" + std::string(_n.name()) + "' is an attribute.");
    }
  };
  return std::visit(cast, _node_or_attribute);
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  const auto name = _arr.node_.name();
  size_t i = 0;
  for (auto node = _arr.node_; node; node = node.next_sibling(name)) {
    if (i == _idx) {
      return InputVarType(node);
    }
  }
  return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType _obj) const noexcept {
  const auto node = _obj.node_.child(_name.c_str());
  if (!node) {
    return rfl::Error("Object contains no field named '" + _name + "'.");
  }
  return InputVarType(node);
}

bool Reader::is_empty(const InputVarType _var) const noexcept {
  const auto wrap = [](const auto& _node) { return !_node; };
  return std::visit(cast_as_node, _var.node_or_attribute_)
      .transform(wrap)
      .value_or(false);
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType _var) const noexcept {
  const auto wrap = [](const auto& _node) { return InputArrayType(_node); };
  return std::visit(cast_as_node, _var.node_or_attribute_).transform(wrap);
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType _var) const noexcept {
  const auto wrap = [](const auto& _node) { return InputObjectType(_node); };
  return std::visit(cast_as_node, _var.node_or_attribute_).transform(wrap);
}

}  // namespace rfl::xml
