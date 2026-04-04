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

#include "rfl/yas/Writer.hpp"

namespace rfl::yas {

Writer::OutputArrayType Writer::add_array_to_map(const std::string_view& _name,
                                                 const size_t _size,
                                                 OutputMapType* _parent) const
    noexcept {
  (*_parent->ar) & std::string(_name);
  (*_parent->ar) & _size;
  return OutputArrayType{_parent->ar};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size, OutputUnionType* _parent) const
    noexcept {
  (*_parent->ar) & _index;
  (*_parent->ar) & _size;
  return OutputArrayType{_parent->ar};
}

Writer::OutputMapType Writer::add_map_to_map(const std::string_view& _name,
                                             const size_t _size,
                                             OutputMapType* _parent) const
    noexcept {
  (*_parent->ar) & std::string(_name);
  (*_parent->ar) & _size;
  return OutputMapType{_parent->ar};
}

Writer::OutputMapType Writer::add_map_to_union(const size_t _index,
                                               const size_t _size,
                                               OutputUnionType* _parent) const
    noexcept {
  (*_parent->ar) & _index;
  (*_parent->ar) & _size;
  return OutputMapType{_parent->ar};
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t, OutputMapType* _parent) const
    noexcept {
  (*_parent->ar) & std::string(_name);
  return OutputObjectType{_parent->ar};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t, OutputUnionType* _parent) const
    noexcept {
  (*_parent->ar) & _index;
  return OutputObjectType{_parent->ar};
}

Writer::OutputUnionType Writer::add_union_to_map(const std::string_view& _name,
                                                 OutputMapType* _parent) const
    noexcept {
  (*_parent->ar) & std::string(_name);
  return OutputUnionType{_parent->ar};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  (*_parent->ar) & _index;
  return OutputUnionType{_parent->ar};
}

Writer::OutputVarType Writer::add_null_to_union(const size_t _index,
                                                OutputUnionType* _parent) const
    noexcept {
  (*_parent->ar) & _index;
  return OutputVarType{};
}

}  // namespace rfl::yas
