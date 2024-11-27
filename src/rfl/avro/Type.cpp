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

#include "rfl/avro/schema/Type.hpp"

namespace rfl::avro::schema {

Type Type::with_name(const std::string& _name) const {
  const auto set_name = [&](const auto& _v) -> ReflectionType {
    using T = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<T, Record>() || std::is_same<T, Enum>()) {
      auto v_with_name = _v;
      v_with_name.name = _name;
      return v_with_name;
    } else {
      return _v;
    }
  };
  return Type{.value = value.visit(set_name)};
}

}  // namespace rfl::avro::schema
