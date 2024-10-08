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

#include "rfl/xml/Writer.hpp"

#include <pugixml.hpp>

namespace rfl::xml {

static constexpr const char* XML_CONTENT = "xml_content";

Writer::Writer(const Ref<pugi::xml_node>& _root, const std::string& _root_name)
    : root_(_root), root_name_(_root_name) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  auto node_child =
      Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
  return OutputArrayType(root_name_, node_child);
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  auto node_child =
      Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
  return OutputObjectType(node_child);
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  auto node_child =
      Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
  return OutputVarType(node_child);
}

Writer::OutputVarType Writer::value_as_root_impl(
    const std::string& _str) const noexcept {
  auto node_child =
      Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
  node_child->append_child(pugi::node_pcdata).set_value(_str.c_str());
  return OutputVarType(node_child);
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return *_parent;
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return OutputArrayType(_name, _parent->node_);
}

Writer::OutputVarType Writer::add_value_to_array_impl(
    const std::string& _str, OutputArrayType* _parent) const noexcept {
  auto node_child = Ref<pugi::xml_node>::make(
      _parent->node_->append_child(_parent->name_.data()));
  node_child->append_child(pugi::node_pcdata).set_value(_str.c_str());
  return OutputVarType(node_child);
}

Writer::OutputVarType Writer::add_value_to_object_impl(
    const std::string_view& _name, const std::string& _str,
    OutputObjectType* _parent, const bool _is_attribute) const noexcept {
  if (_is_attribute) {
    _parent->node_->append_attribute(_name.data()) = _str.c_str();
    return OutputVarType(_parent->node_);
  } else if (_name == XML_CONTENT) {
    _parent->node_->append_child(pugi::node_pcdata).set_value(_str.c_str());
    return OutputVarType(_parent->node_);
  } else {
    auto node_child =
        Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
    node_child->append_child(pugi::node_pcdata).set_value(_str.c_str());
    return OutputVarType(node_child);
  }
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  auto node_child = Ref<pugi::xml_node>::make(
      _parent->node_->append_child(_parent->name_.data()));
  return OutputObjectType(node_child);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  auto node_child =
      Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
  return OutputObjectType(node_child);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  auto node_child = Ref<pugi::xml_node>::make(
      _parent->node_->append_child(_parent->name_.data()));
  return OutputVarType(node_child);
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent,
    const bool _is_attribute) const noexcept {
  if (_is_attribute) {
    return OutputVarType(_parent->node_);
  } else if (_name == XML_CONTENT) {
    return OutputVarType(_parent->node_);
  } else {
    auto node_child =
        Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
    return OutputVarType(node_child);
  }
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {}

void Writer::end_object(OutputObjectType* _obj) const noexcept {}

}  // namespace rfl::xml
