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

#include <functional>
#include <type_traits>

#include "rfl/internal/strings/strings.hpp"

namespace rfl::capnproto::schema {

/// Cap'n proto has a somewhat weird way of handling union fields, please refer
/// to the schema for further explanation. This is a workaround that ensures
/// that we can also support complex, nested unions.
void handle_fields_in_structs_or_unions(const auto& _struct_or_union,
                                        const size_t _indent,
                                        const std::string& _namespace,
                                        std::ostream* _os, size_t* _ix) {
  for (const auto& pair : _struct_or_union.fields) {
    const auto name = pair.first;
    // Because of the way Cap'n proto handles unions, we need a special case for
    // them.
    pair.second.reflection().visit([&](const auto& _r) {
      using R = std::remove_cvref_t<decltype(_r)>;
      if constexpr (std::is_same<R, Type::Union>()) {
        // Special case: Union field.
        *_os << std::string(_indent * 2, ' ') << name << " :union {"
             << std::endl;
        for (size_t i = 0; i < _r.fields.size(); ++i) {
          _r.fields[i].second.reflection().visit([&](const auto& _union_field) {
            using U = std::remove_cvref_t<decltype(_union_field)>;
            if constexpr (std::is_same<U, Type::Union>()) {
              *_os << std::string(_indent * 2 + 2, ' ')
                   << internal::strings::to_camel_case(_namespace + name + "_" +
                                                       _r.fields[i].first)
                   << " :union {" << std::endl;
              handle_fields_in_structs_or_unions(
                  _union_field, _indent + 2,
                  _namespace + name + "_" + _r.fields[i].first + "_", _os, _ix);
              *_os << std::string(_indent * 2 + 2, ' ') << "}" << std::endl;

            } else {
              *_os << std::string(_indent * 2 + 2, ' ')
                   << internal::strings::to_camel_case(_namespace + name + "_" +
                                                       _r.fields[i].first)
                   << " @" << (*_ix)++ << " :" << _union_field << ";"
                   << std::endl;
            }
          });
        }
        *_os << std::string(_indent * 2, ' ') << "}" << std::endl;

      } else {
        // Standard case: Non-union field.
        *_os << std::string(_indent * 2, ' ')
             << internal::strings::to_camel_case(_namespace + name) << " @"
             << (*_ix)++ << " :" << _r << ";" << std::endl;
      }
    });
  }
}

Type Type::with_name(const std::string& _name) const {
  const auto set_name = [&](const auto& _v) -> ReflectionType {
    using T = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<T, Struct>() || std::is_same<T, Union>() ||
                  std::is_same<T, Enum>()) {
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

std::ostream& operator<<(std::ostream& _os, const Type::Enum& _e) {
  _os << "enum " << _e.name << " {" << std::endl;
  for (size_t i = 0; i < _e.fields.size(); ++i) {
    _os << "  " << internal::strings::to_camel_case(_e.fields[i]) << " @" << i
        << ";" << std::endl;
  }
  return _os << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::List& _l) {
  return _os << "List(" << *_l.type << ")";
}

std::ostream& operator<<(std::ostream& _os, const Type::Map& _m) {
  return _os << "Map(" << *_m.type << ")";
}

std::ostream& operator<<(std::ostream& _os, const Type::Text&) {
  return _os << "Text";
}

std::ostream& operator<<(std::ostream& _os, const Type::Struct& _s) {
  _os << "struct " << _s.name << " {" << std::endl;
  size_t ix = 0;
  handle_fields_in_structs_or_unions(_s, 1, "", &_os, &ix);
  return _os << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::Union& _u) {
  _os << "struct " << _u.name << " {" << std::endl << "  union {" << std::endl;
  size_t ix = 0;
  handle_fields_in_structs_or_unions(_u, 2, "", &_os, &ix);
  return _os << "  }" << std::endl << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::Reference& _r) {
  return _os << internal::strings::to_pascal_case(_r.type_name);
}

std::ostream& operator<<(std::ostream& _os, const Type& _t) {
  return _t.reflection().visit(
      [&](const auto& _r) -> std::ostream& { return _os << _r; });
}

}  // namespace rfl::capnproto::schema
