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

#include "rfl/capnproto/schema/Type.hpp"

#include <type_traits>

#include "rfl/internal/strings/to_pascal_case.hpp"

namespace rfl::capnproto::schema {

Type Type::with_name(const std::string& _name) const {
  const auto set_name = [&](const auto& _v) -> ReflectionType {
    using T = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<T, Struct>()) {
      auto v_with_name = _v;
      v_with_name.name = _name;
      return v_with_name;
    } else {
      return _v;
    }
  };
  return Type{.value = value.visit(set_name)};
}

std::ostream& operator<<(std::ostream& _os, const Type::Void&) {
  return _os << "Void";
}

std::ostream& operator<<(std::ostream& _os, const Type::Bool&) {
  return _os << "Bool";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int8&) {
  return _os << "Int8";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int16&) {
  return _os << "Int16";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int32&) {
  return _os << "Int32";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int64&) {
  return _os << "Int64";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt8&) {
  return _os << "UInt8";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt16&) {
  return _os << "UInt16";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt32&) {
  return _os << "UInt32";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt64&) {
  return _os << "UInt64";
}

std::ostream& operator<<(std::ostream& _os, const Type::Float32&) {
  return _os << "Float32";
}

std::ostream& operator<<(std::ostream& _os, const Type::Float64&) {
  return _os << "Float64";
}

std::ostream& operator<<(std::ostream& _os, const Type::Data&) {
  return _os << "Data";
}

std::ostream& operator<<(std::ostream& _os, const Type::Optional&) {
  return _os << "TODO";
}

std::ostream& operator<<(std::ostream& _os, const Type::Text&) {
  return _os << "Text";
}

std::ostream& operator<<(std::ostream& _os, const Type::Struct& _s) {
  _os << "struct " << _s.name << " {" << std::endl;
  size_t ix = 0;
  for (const auto& [name, type] : _s.fields) {
    type.reflection().visit([&](const auto& _r) {
      using R = std::remove_cvref_t<decltype(_r)>;
      if constexpr (std::is_same<R, Type::Optional>()) {
        _os << "  " << name << " :union {" << std::endl
            << "    some @" << ix++ << " :" << *_r.type << ";" << std::endl
            << "    none @" << ix++ << " :Void;" << std::endl
            << "   }" << std::endl;
      } else {
        _os << "  " << name << " @" << ix++ << " :" << _r << ";" << std::endl;
      }
    });
  }
  return _os << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::List& _l) {
  return _os << "List(" << *_l.type << ")";
}

std::ostream& operator<<(std::ostream& _os, const Type::Reference& _r) {
  return _os << internal::strings::to_pascal_case(_r.type_name);
}

std::ostream& operator<<(std::ostream& _os, const Type& _t) {
  return _t.reflection().visit(
      [&](const auto& _r) -> std::ostream& { return _os << _r; });
}

}  // namespace rfl::capnproto::schema
