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

#include "rfl/env/Writer.hpp"

#include <cstdlib>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>

namespace rfl::env {

int portable_setenv(const char* name, const char* value) {
#ifdef _WIN32
  return _putenv_s(name, value);
#else
  return setenv(name, value, 1);
#endif
}

Writer::Writer() {}

Writer::OutputArrayType Writer::array_as_root(
    const size_t /*_size*/) const noexcept {
  return OutputArrayType{.prefix = ""};
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t /*_size*/) const noexcept {
  return OutputObjectType{.prefix = ""};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t /*_size*/, OutputArrayType* _parent) const {
  const auto arr =
      OutputArrayType{.prefix = _parent->prefix +
                                std::to_string(_parent->values->size()) + "_"};
  _parent->values->emplace_back(arr);
  return arr;
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  const auto arr =
      OutputArrayType{.prefix = _parent->prefix + std::string(_name) + "_"};
  _parent->fields->emplace(std::string(_name), arr);
  return arr;
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  const auto obj =
      OutputObjectType{.prefix = _parent->prefix + std::string(_name) + "_"};
  _parent->fields->emplace(std::string(_name), obj);
  return obj;
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t /*_size*/, OutputArrayType* _parent) const {
  const auto obj =
      OutputObjectType{.prefix = _parent->prefix +
                                 std::to_string(_parent->values->size()) + "_"};
  _parent->values->emplace_back(obj);
  return obj;
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const {
  const auto var = OutputVarType{};
  _parent->values->emplace_back(var);
  return var;
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  const auto var = OutputVarType{};
  _parent->fields->emplace(std::string(_name), var);
  return var;
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {
  const auto& arr = *_arr;
  for (size_t i = 0; i < arr.values->size(); ++i) {
    const auto& value = (*arr.values)[i];
    value.visit([&](const auto& _v) {
      using T = std::remove_cvref_t<decltype(_v)>;
      if constexpr (std::is_same_v<T, OutputVarType>) {
        portable_setenv((arr.prefix + std::to_string(i)).c_str(),
                        _v.value.c_str());

      } else if constexpr (std::is_same_v<T, OutputArrayType>) {
        // Do nothing for arrays, as end_array should have already
        // processed their fields recursively.

      } else if constexpr (std::is_same_v<T, OutputObjectType>) {
        // Do nothing for objects, as end_object should have already
        // processed their fields recursively.

      } else {
        static_assert(always_false_v<T>,
                      "Unsupported type in OutputArrayType.");
      }
    });
  }
}

void Writer::end_object(OutputObjectType* _obj) const noexcept {
  const auto& obj = *_obj;
  for (const auto& [key, value] : *obj.fields) {
    // Explicit capture of key necessary, because of issues with Clang 15.
    value.visit([&, key](const auto& _v) {
      using T = std::remove_cvref_t<decltype(_v)>;
      if constexpr (std::is_same_v<T, OutputVarType>) {
        portable_setenv((obj.prefix + key).c_str(), _v.value.c_str());

      } else if constexpr (std::is_same_v<T, OutputArrayType>) {
        // Do nothing for arrays, as end_array should have already
        // processed their fields recursively.

      } else if constexpr (std::is_same_v<T, OutputObjectType>) {
        // Do nothing for objects, as end_object should have already
        // processed their fields recursively.

      } else {
        static_assert(always_false_v<T>,
                      "Unsupported type in OutputObjectType.");
      }
    });
  }
}

}  // namespace rfl::env
