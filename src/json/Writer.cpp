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

#include "rfl/json/Writer.hpp"

namespace rfl::json {

Writer::Writer(yyjson_mut_doc* _doc) : doc_(_doc) {}

Writer::OutputArrayType Writer::array_as_root(const size_t) const noexcept {
  const auto arr = yyjson_mut_arr(doc_);
  yyjson_mut_doc_set_root(doc_, arr);
  return OutputArrayType(arr);
}

Writer::OutputObjectType Writer::object_as_root(const size_t) const noexcept {
  const auto obj = yyjson_mut_obj(doc_);
  yyjson_mut_doc_set_root(doc_, obj);
  return OutputObjectType(obj);
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  const auto null = yyjson_mut_null(doc_);
  yyjson_mut_doc_set_root(doc_, null);
  return OutputVarType(null);
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t, OutputArrayType* _parent) const noexcept {
  const auto arr = yyjson_mut_arr(doc_);
  yyjson_mut_arr_add_val(_parent->val_, arr);
  return OutputArrayType(arr);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t,
    OutputObjectType* _parent) const noexcept {
  const auto arr = yyjson_mut_arr(doc_);
  yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()), arr);
  return OutputArrayType(arr);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t, OutputArrayType* _parent) const noexcept {
  const auto obj = yyjson_mut_obj(doc_);
  yyjson_mut_arr_add_val(_parent->val_, obj);
  return OutputObjectType(obj);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t,
    OutputObjectType* _parent) const noexcept {
  const auto obj = yyjson_mut_obj(doc_);
  yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()), obj);
  return OutputObjectType(obj);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  const auto null = yyjson_mut_null(doc_);
  yyjson_mut_arr_add_val(_parent->val_, null);
  return OutputVarType(null);
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  const auto null = yyjson_mut_null(doc_);
  yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()),
                     null);
  return OutputVarType(null);
}

void Writer::end_array(OutputArrayType*) const noexcept {}

void Writer::end_object(OutputObjectType*) const noexcept {}

}  // namespace rfl::json
