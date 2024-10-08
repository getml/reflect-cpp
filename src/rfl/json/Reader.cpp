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

#include "rfl/json/Reader.hpp"

namespace rfl::json {

rfl::Result<Reader::InputVarType> Reader::get_field_from_array(
    const size_t _idx, const InputArrayType _arr) const noexcept {
  const auto var = InputVarType(yyjson_arr_get(_arr.val_, _idx));
  if (!var.val_) {
    return rfl::Error("Index " + std::to_string(_idx) + " of of bounds.");
  }
  return var;
}

rfl::Result<Reader::InputVarType> Reader::get_field_from_object(
    const std::string& _name, const InputObjectType _obj) const noexcept {
  const auto var = InputVarType(yyjson_obj_get(_obj.val_, _name.c_str()));
  if (!var.val_) {
    return rfl::Error("Object contains no field named '" + _name + "'.");
  }
  return var;
}

bool Reader::is_empty(const InputVarType _var) const noexcept {
  return !_var.val_ || yyjson_is_null(_var.val_);
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType _var) const noexcept {
  if (!yyjson_is_arr(_var.val_)) {
    return rfl::Error("Could not cast to array!");
  }
  return InputArrayType(_var.val_);
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType _var) const noexcept {
  if (!yyjson_is_obj(_var.val_)) {
    return rfl::Error("Could not cast to object!");
  }
  return InputObjectType(_var.val_);
}

}  // namespace rfl::json
