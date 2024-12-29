#ifndef RFL_CAPNPROTO_READER_HPP_
#define RFL_CAPNPROTO_READER_HPP_

#include <capnp/dynamic.h>

#include <cstddef>
#include <exception>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::capnproto {

struct Reader {
  struct CapNProtoInputArray {
    capnp::DynamicList::Reader val_;
  };

  struct CapNProtoInputMap {
    capnp::DynamicList::Reader val_;
  };

  struct CapNProtoInputObject {
    capnp::DynamicStruct::Reader val_;
  };

  struct CapNProtoInputUnion {
    capnp::DynamicStruct::Reader val_;
  };

  struct CapNProtoInputVar {
    capnp::DynamicValue::Reader val_;
  };

  using InputArrayType = CapNProtoInputArray;
  using InputObjectType = CapNProtoInputObject;
  using InputMapType = CapNProtoInputMap;
  using InputUnionType = CapNProtoInputUnion;
  using InputVarType = CapNProtoInputVar;

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_capnproto_obj(var); });

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = _var.val_.getType();
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (type != capnp::DynamicValue::TEXT) {
        return Error("Could not cast to string.");
      }
      return std::string(_var.val_.as<capnp::Text>().cStr());
      // TODO
      /*} else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                        rfl::Bytestring>()) {
        const void* ptr = nullptr;
        size_t size = 0;
        const auto err = capnproto_value_get_bytes(_var.val_, &ptr, &size);
        if (err) {
          return Error("Could not cast to bytestring.");
        }
        return rfl::Bytestring(static_cast<const std::byte*>(ptr), size - 1);*/
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != capnp::DynamicValue::BOOL) {
        return rfl::Error("Could not cast to boolean.");
      }
      return _var.val_.as<bool>();

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      switch (type) {
        case capnp::DynamicValue::INT:
          return static_cast<T>(_var.val_.as<int64_t>());
        case capnp::DynamicValue::UINT:
          return static_cast<T>(_var.val_.as<uint64_t>());
        case capnp::DynamicValue::FLOAT:
          return static_cast<T>(_var.val_.as<double>());
      }
      return rfl::Error(
          "Could not cast to numeric value. The type must be integral, "
          "float or double.");

      // TODO
      /*} else if constexpr (internal::is_literal_v<T>) {
        int value = 0;
        const auto err = capnproto_value_get_enum(_var.val_, &value);
        if (err) {
          return Error("Could not cast to enum.");
        }
        return std::remove_cvref_t<T>::from_value(
            static_cast<typename
        std::remove_cvref_t<T>::ValueType>(value));*/
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept;

  rfl::Result<InputUnionType> to_union(const InputVarType& _var) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (auto element : _arr.val_) {
      const auto err = _array_reader.read(InputVarType{std::move(element)});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class MapReader>
  std::optional<Error> read_map(const MapReader& _map_reader,
                                const InputMapType& _map) const noexcept {
    // TODO
    /*size_t size = 0;
  capnproto_value_get_size(_map.val_, &size);
  for (size_t ix = 0; ix < size; ++ix) {
    capnproto_value_t element;
    const char* key = nullptr;
    capnproto_value_get_by_index(_map.val_, ix, &element, &key);
    _map_reader.read(std::string_view(key), InputVarType{&element});
  }*/
    return std::nullopt;
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (auto field : _obj.val_.getSchema().getFields()) {
      if (!_obj.val_.has(field)) {
        continue;
      }
      _object_reader.read(field.getProto().getName().cStr(),
                          InputVarType{_obj.val_.get(field)});
    }
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    // TODO
    /*int disc = 0;
    auto err = capnproto_value_get_discriminant(_union.val_, &disc);
    if (err) {
      return Error("Could not get the discriminant.");
    }
    capnproto_value_t value;
    err = capnproto_value_get_current_branch(_union.val_, &value);
    if (err) {
      return Error("Could not cast the union type.");
    }
    return UnionReaderType::read(*this, static_cast<size_t>(disc),
                                 InputVarType{&value});*/
    return Error("TODO");
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_capnproto_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }
};

}  // namespace rfl::capnproto

#endif
