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

#include "rfl/capnproto/to_schema.hpp"

#include <sstream>

#include "rfl/capnproto/schema/Type.hpp"
#include "rfl/internal/strings/split.hpp"
#include "rfl/internal/strings/to_pascal_case.hpp"
#include "rfl/json.hpp"
#include "rfl/parsing/schemaful/tuple_to_object.hpp"

namespace rfl::capnproto {

inline bool is_named_type(const parsing::schema::Type& _type) {
  return _type.variant_.visit([&](const auto& _v) -> bool {
    using T = std::remove_cvref_t<decltype(_v)>;
    return std::is_same<T, parsing::schema::Type::Object>() ||
           std::is_same<T, parsing::schema::Type::Literal>();
  });
}

schema::Type type_to_capnproto_schema_type(
    const parsing::schema::Type& _type,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    size_t* _num_unnamed) {
  auto handle_variant = [&](const auto& _t) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Boolean>()) {
      return schema::Type{.value = schema::Type::Bool{}};

    } else if constexpr (std::is_same<T, Type::Bytestring>()) {
      return schema::Type{.value = schema::Type::Data{}};

    } else if constexpr (std::is_same<T, Type::Int32>() ||
                         std::is_same<T, Type::Integer>()) {
      return schema::Type{.value = schema::Type::Int32{}};

    } else if constexpr (std::is_same<T, Type::Int64>()) {
      return schema::Type{.value = schema::Type::Int64{}};

    } else if constexpr (std::is_same<T, Type::UInt32>()) {
      return schema::Type{.value = schema::Type::UInt32{}};

    } else if constexpr (std::is_same<T, Type::UInt64>()) {
      return schema::Type{.value = schema::Type::UInt64{}};

    } else if constexpr (std::is_same<T, Type::Float>()) {
      return schema::Type{.value = schema::Type::Float32{}};

    } else if constexpr (std::is_same<T, Type::Double>()) {
      return schema::Type{.value = schema::Type::Float64{}};

    } else if constexpr (std::is_same<T, Type::String>()) {
      return schema::Type{.value = schema::Type::Text{}};

    } else if constexpr (std::is_same<T, Type::AnyOf>()) {
      auto value = schema::Type::Variant{};
      size_t i = 1;
      for (const auto& type : _t.types_) {
        value.fields.push_back(std::make_pair(
            std::string("Opt" + std::to_string(i++)),
            type_to_capnproto_schema_type(type, _definitions, _num_unnamed)));
      }
      return schema::Type{.value = value};

    } else if constexpr (std::is_same<T, Type::Description>()) {
      return type_to_capnproto_schema_type(*_t.type_, _definitions,
                                           _num_unnamed);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{
          .value = schema::Type::List{
              .type = Ref<schema::Type>::make(type_to_capnproto_schema_type(
                  *_t.type_, _definitions, _num_unnamed))}};

    } else if constexpr (std::is_same<T, Type::Literal>()) {
      // TODO
      return schema::Type{.value = schema::Type::Void{}};
      /*return schema::Type{
          .value = schema::Type::Enum{.name = std::string("unnamed_") +
                                              std::to_string(++(*_num_unnamed)),
                                      .symbols = _t.values_}};*/

    } else if constexpr (std::is_same<T, Type::Object>()) {
      auto struct_schema = schema::Type::Struct{
          .name = std::string("Unnamed") + std::to_string(++(*_num_unnamed))};
      for (const auto& [k, v] : _t.types_) {
        struct_schema.fields.push_back(std::make_pair(
            k, type_to_capnproto_schema_type(v, _definitions, _num_unnamed)));
      }
      return schema::Type{.value = struct_schema};

    } else if constexpr (std::is_same<T, Type::Optional>()) {
      return schema::Type{
          .value = schema::Type::Variant{
              .fields = std::vector(
                  {std::make_pair(std::string("Some"),
                                  type_to_capnproto_schema_type(
                                      *_t.type_, _definitions, _num_unnamed)),
                   std::make_pair(std::string("None"),
                                  schema::Type{schema::Type::Void{}})})}};

    } else if constexpr (std::is_same<T, Type::Reference>()) {
      return schema::Type{.value =
                              schema::Type::Reference{.type_name = _t.name_}};
    } else if constexpr (std::is_same<T, Type::StringMap>()) {
      // TODO
      return schema::Type{.value = schema::Type::Void{}};
      /*return schema::Type{
          .value = schema::Type::Map{
              .values = Ref<schema::Type>::make(type_to_capnproto_schema_type(
                  *_t.value_type_, _definitions,
                  _num_unnamed))}};*/
    } else if constexpr (std::is_same<T, Type::Tuple>()) {
      return type_to_capnproto_schema_type(
          Type{parsing::schemaful::tuple_to_object(_t)}, _definitions,
          _num_unnamed);

    } else if constexpr (std::is_same<T, Type::TypedArray>()) {
      return schema::Type{
          .value = schema::Type::List{
              .type = Ref<schema::Type>::make(type_to_capnproto_schema_type(
                  *_t.type_, _definitions, _num_unnamed))}};

    } else if constexpr (std::is_same<T, Type::Validated>()) {
      // Cap'n Proto knows no validation.
      return type_to_capnproto_schema_type(*_t.type_, _definitions,
                                           _num_unnamed);
    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  };

  return rfl::visit(handle_variant, _type.variant_);
}

std::string to_string_representation(
    const parsing::schema::Definition& _internal_schema) {
  size_t num_unnamed = 0;
  std::stringstream sstream;
  // TODO: ID is hardcoded.
  sstream << "@0xdbb9ad1f14bf0b36;" << std::endl << std::endl;
  for (const auto& [name, def] : _internal_schema.definitions_) {
    sstream << type_to_capnproto_schema_type(def, _internal_schema.definitions_,
                                             &num_unnamed)
                   .with_name(internal::strings::to_pascal_case(name))
            << std::endl
            << std::endl;
  }
  return sstream.str();
}

}  // namespace rfl::capnproto
