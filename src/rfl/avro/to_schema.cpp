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

#include "rfl/avro/to_schema.hpp"

#include "rfl/avro/schema/Type.hpp"
#include "rfl/json.hpp"

namespace rfl::avro {

schema::Type type_to_avro_schema_type(const parsing::schema::Type& _type) {
  auto handle_variant = [](const auto& _t) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Boolean>()) {
      return schema::Type{.value = schema::Type::Boolean{}};

    } else if constexpr (std::is_same<T, Type::Int32>() ||
                         std::is_same<T, Type::Int64>() ||
                         std::is_same<T, Type::UInt32>() ||
                         std::is_same<T, Type::UInt64>() ||
                         std::is_same<T, Type::Integer>()) {
      return schema::Type{.value = schema::Type::Int{}};

    } else if constexpr (std::is_same<T, Type::Int64>() ||
                         std::is_same<T, Type::UInt32>() ||
                         std::is_same<T, Type::UInt64>()) {
      return schema::Type{.value = schema::Type::Long{}};

    } else if constexpr (std::is_same<T, Type::Float>()) {
      return schema::Type{.value = schema::Type::Float{}};

    } else if constexpr (std::is_same<T, Type::Double>()) {
      return schema::Type{.value = schema::Type::Double{}};

    } else if constexpr (std::is_same<T, Type::String>()) {
      return schema::Type{.value = schema::Type::String{}};

    } else if constexpr (std::is_same<T, Type::AnyOf>()) {
      auto any_of = std::vector<schema::Type>();
      for (const auto& t : _t.types_) {
        any_of.emplace_back(type_to_avro_schema_type(t));
      }
      return schema::Type{.value = any_of};

    } else if constexpr (std::is_same<T, Type::Description>()) {
      // TODO: Return descriptions
      return type_to_avro_schema_type(*_t.type_);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{.value = schema::Type::Array{
                              .items = Ref<schema::Type>::make(
                                  type_to_avro_schema_type(*_t.type_))}};

    } else if constexpr (std::is_same<T, Type::Literal>()) {
      return schema::Type{.value = schema::Type::Enum{.symbols = _t.values_}};

    } else if constexpr (std::is_same<T, Type::Object>()) {
      auto record = schema::Type::Record{};
      for (const auto& [k, v] : _t.types_) {
        record.fields.push_back(schema::Type::RecordField{
            .name = k,
            .type = Ref<schema::Type>::make(type_to_avro_schema_type(v))});
      }
      return schema::Type{.value = record};

    } else if constexpr (std::is_same<T, Type::Optional>()) {
      return schema::Type{.value = std::vector<schema::Type>(
                              {type_to_avro_schema_type(*_t.type_),
                               schema::Type{schema::Type::Null{}}})};

    } else if constexpr (std::is_same<T, Type::Reference>()) {
      return schema::Type{.value = _t.name_};

    } else if constexpr (std::is_same<T, Type::StringMap>()) {
      return schema::Type{.value = schema::Type::Map{
                              .values = Ref<schema::Type>::make(
                                  type_to_avro_schema_type(*_t.value_type_))}};

    } else if constexpr (std::is_same<T, Type::Tuple>()) {
      // TODO: Handle tuples.
      return schema::Type{.value = schema::Type::Record{}};

    } else if constexpr (std::is_same<T, Type::TypedArray>()) {
      return schema::Type{.value = schema::Type::Array{
                              .items = Ref<schema::Type>::make(
                                  type_to_avro_schema_type(*_t.type_))}};

    } else if constexpr (std::is_same<T, Type::Validated>()) {
      // Avro knows no validation.
      return type_to_avro_schema_type(*_t.type_);

    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  };

  return rfl::visit(handle_variant, _type.variant_);
}

std::string to_json_representation(
    const parsing::schema::Definition& internal_schema) {
  const auto avro_schema = type_to_avro_schema_type(internal_schema.root_);
  return rfl::json::write(avro_schema);
}

}  // namespace rfl::avro
