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

#include "rfl/generic/Writer.hpp"

namespace rfl::generic {

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  root_ = Generic::Array();
  return OutputArray{get_if<Generic::Array>(&root_.variant())};
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  root_ = Generic::Object();
  return OutputObject{get_if<Generic::Object>(&root_.variant())};
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  root_ = Generic::Null;
  return root_;
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  _parent->val_->push_back(Generic(Generic::Array()));
  return OutputArrayType{
      get_if<Generic::Array>(&_parent->val_->rbegin()->variant())};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  _parent->val_->insert(_name, Generic(Generic::Array()));
  return OutputArrayType{
      get_if<Generic::Array>(&_parent->val_->rbegin()->second.variant())};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  _parent->val_->push_back(Generic(Generic::Object()));
  return OutputObjectType{
      get_if<Generic::Object>(&_parent->val_->rbegin()->variant())};
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  _parent->val_->insert(_name, Generic(Generic::Object()));
  return OutputObjectType{
      get_if<Generic::Object>(&_parent->val_->rbegin()->second.variant())};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  _parent->val_->push_back(Generic(Generic::Null));
  return Generic::Null;
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  _parent->val_->insert(_name, Generic(Generic::Null));
  return Generic::Null;
}

}  // namespace rfl::generic
