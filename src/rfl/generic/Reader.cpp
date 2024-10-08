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

#include "rfl/generic/Reader.hpp"

namespace rfl::generic {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType& _arr) const noexcept {
  if (_idx >= _arr.size()) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return _arr[_idx];
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType& _obj) const noexcept {
  return _obj.get(_name);
}

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return _var.is_null();
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  return _var.to_array();
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  return _var.to_object();
}

}  // namespace rfl::generic
