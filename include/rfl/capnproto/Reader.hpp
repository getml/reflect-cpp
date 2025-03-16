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
#include "../internal/ptr_cast.hpp"

namespace rfl::capnproto {

class Reader {
 public:
  struct CapNProtoInputArray {
    capnp::DynamicList::Reader val_;
  };

  struct CapNProtoInputMap {
    capnp::DynamicStruct::Reader val_;
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

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.val_.getType() == capnp::DynamicValue::VOID;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    const auto type = _var.val_.getType();
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (type != capnp::DynamicValue::TEXT) {
        return error("Could not cast to string.");
      }
      return std::string(_var.val_.as<capnp::Text>().cStr());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      if (type != capnp::DynamicValue::DATA) {
        return error("Could not cast to bytestring.");
      }
      const auto data = _var.val_.as<capnp::Data>();
      const auto begin = internal::ptr_cast<const std::byte*>(data.begin());
      return rfl::Bytestring(begin, begin + data.size());

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (type != capnp::DynamicValue::BOOL) {
        return error("Could not cast to boolean.");
      }
      return _var.val_.as<bool>();

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      if (type != capnp::DynamicValue::FLOAT) {
        return error("Could not cast to float.");
      }
      return static_cast<T>(_var.val_.as<double>());

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      switch (type) {
        case capnp::DynamicValue::INT:
          return static_cast<T>(_var.val_.as<int64_t>());

        case capnp::DynamicValue::UINT:
          return static_cast<T>(_var.val_.as<uint64_t>());

        default:
          return error("Could not cast to an integer.");
      }

    } else if constexpr (internal::is_literal_v<T>) {
      if (type != capnp::DynamicValue::ENUM) {
        return error("Could not cast to an enum.");
      }
      return T::from_value(_var.val_.as<capnp::DynamicEnum>().getRaw());

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (_var.val_.getType() != capnp::DynamicValue::LIST) {
      return error("Could not cast to a list.");
    }
    return InputArrayType{_var.val_.as<capnp::DynamicList>()};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (_var.val_.getType() != capnp::DynamicValue::STRUCT) {
      return error("Could not cast to a struct.");
    }
    return InputObjectType{_var.val_.as<capnp::DynamicStruct>()};
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    if (_var.val_.getType() != capnp::DynamicValue::STRUCT) {
      return error("Could not cast to a map.");
    }
    return InputMapType{_var.val_.as<capnp::DynamicStruct>()};
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    if (_var.val_.getType() != capnp::DynamicValue::STRUCT) {
      return error("Could not cast to a struct.");
    }
    return InputUnionType{_var.val_.as<capnp::DynamicStruct>()};
  }

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
    try {
      const auto entries = _map.val_.get("entries").as<capnp::DynamicList>();
      for (auto entry : entries) {
        auto s = entry.template as<capnp::DynamicStruct>();
        const char* key = s.get("key").as<capnp::Text>().cStr();
        _map_reader.read(std::string_view(key), InputVarType{s.get("value")});
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error{e.what()};
    }
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    int i = 0;
    for (auto field : _obj.val_.getSchema().getFields()) {
      _object_reader.read(i++, InputVarType{_obj.val_.get(field)});
    }
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    const auto opt_pair = identify_discriminant(_union);
    if (!opt_pair) {
      return error("Could not get the discriminant.");
    }
    const auto& [field, disc] = *opt_pair;
    return UnionReaderType::read(*this, disc,
                                 InputVarType{_union.val_.get(field)});
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_capnproto_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

 private:
  std::optional<std::pair<capnp::StructSchema::Field, size_t>>
  identify_discriminant(const InputUnionType& _union) const noexcept;
};

}  // namespace rfl::capnproto

#endif
