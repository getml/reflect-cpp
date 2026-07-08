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

static constexpr const char* XML_CONTENT = "xml_content";

Writer::Writer() {}

Writer::OutputObjectType Writer::object_as_root(
    const size_t /*_size*/) const noexcept {
  return OutputObjectType{.prefix = ""};
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  const auto arr = OutputArrayType{};
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

void Writer::end_array(OutputArrayType* /*_arr*/) const {}

void Writer::end_object(OutputObjectType* _obj) const {
  const auto& obj = *_obj;
  for (const auto& [key, value] : *obj.fields) {
    value.visit([&](const auto& _v) {
      using T = std::remove_cvref_t<decltype(_v)>;
      if constexpr (std::is_same_v<T, OutputVarType>) {
        std::setenv((obj.prefix + key).c_str(), _v.value.c_str(), 1);

      } else if constexpr (std::is_same_v<T, OutputArrayType>) {
        const auto arr_str = [&]() {
          std::string result;
          for (const auto& val : *_v.values) {
            if (!result.empty()) {
              result += ",";
            }
            result += val.value;
          }
          return result;
        }();
        std::setenv((obj.prefix + key).c_str(), arr_str.c_str(), 1);

      } else if constexpr (std::is_same_v<T, OutputObjectType>) {
        // Do nothing for objects, as end_object should have already processed
        // their fields recursively.

      } else {
        static_assert(always_false_v<T>,
                      "Unsupported type in OutputObjectType.");
      }
    });
  }
}

}  // namespace rfl::env
