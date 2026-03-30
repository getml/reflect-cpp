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

#include "rfl/cereal/Writer.hpp"

namespace rfl::cereal {

Writer::Writer(CerealArchive* _archive) : archive_(_archive) {}

Writer::OutputArrayType Writer::array_as_root(const size_t _size) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputArrayType{};
}

Writer::OutputMapType Writer::map_as_root(const size_t _size) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputMapType{};
}

Writer::OutputObjectType Writer::object_as_root(const size_t _size) const {
  return OutputObjectType{};
}

Writer::OutputUnionType Writer::union_as_root() const {
  return OutputUnionType{};
}

Writer::OutputVarType Writer::null_as_root() const {
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputArrayType{};
}

Writer::OutputArrayType Writer::add_array_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const {
  add_string_view(_name);
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputArrayType{};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputArrayType{};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size, OutputUnionType* _parent) const {
  (*archive_)(_index);
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputArrayType{};
}

Writer::OutputMapType Writer::add_map_to_array(
    const size_t _size, OutputArrayType* _parent) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputMapType{};
}

Writer::OutputMapType Writer::add_map_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const {
  (*archive_)(std::string(_name));
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputMapType{};
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputMapType{};
}

Writer::OutputMapType Writer::add_map_to_union(
    const size_t _index, const size_t _size, OutputUnionType* _parent) const {
  (*archive_)(_index);
  (*archive_)(::cereal::make_size_tag(_size));
  return OutputMapType{};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const {
  return OutputObjectType{};
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const {
  add_string_view(_name);
  return OutputObjectType{};
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  return OutputObjectType{};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size, OutputUnionType* _parent) const {
  (*archive_)(_index);
  return OutputObjectType{};
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const {
  return OutputUnionType{};
}

Writer::OutputUnionType Writer::add_union_to_map(
    const std::string_view& _name, OutputMapType* _parent) const {
  add_string_view(_name);
  return OutputUnionType{};
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  return OutputUnionType{};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  (*archive_)(_index);
  return OutputUnionType{};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const {
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_map(
    const std::string_view& _name, OutputMapType* _parent) const {
  add_string_view(_name);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  (*archive_)(_index);
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {}

void Writer::end_map(OutputMapType* _map) const noexcept {}

void Writer::end_object(OutputObjectType* _obj) const noexcept {}

void Writer::add_string_view(const std::string_view& _str) const {
  (*archive_)(::cereal::make_size_tag(_str.size()));
  (*archive_)(::cereal::binary_data(_str.data(), _str.size()));
}

}  // namespace rfl::cereal
