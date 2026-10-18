#ifndef RFL_CAPNPROTO_READER_HPP_
#define RFL_CAPNPROTO_READER_HPP_

#include <exception>
#include <optional>
#include <string>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/ptr_cast.hpp"
#include "NativeHandle.hpp"

namespace rfl::capnproto {

class RFL_API Reader {
 public:
  struct CapNProtoInputArray {
    detail::ListReader val_;
  };

  struct CapNProtoInputMap {
    detail::StructReader val_;
  };

  struct CapNProtoInputObject {
    detail::StructReader val_;
  };

  struct CapNProtoInputUnion {
    detail::StructReader val_;
  };

  struct CapNProtoInputVar {
    detail::ValueReader val_;
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
  Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<U, std::string>)
      return read_string(_var);
    else if constexpr (std::is_same_v<U, Bytestring> ||
                       std::is_same_v<U, Vectorstring>) {
      return read_bytes(_var, std::is_same_v<U, Vectorstring>)
          .transform([](const auto& _bytes) {
            using Value = typename U::value_type;
            const auto begin = internal::ptr_cast<const Value*>(_bytes.data());
            return U(begin, begin + _bytes.size());
          });
    } else if constexpr (std::is_same_v<U, bool>)
      return read_bool(_var);
    else if constexpr (std::is_floating_point_v<U>)
      return read_double(_var).transform(
          [](auto _v) { return static_cast<T>(_v); });
    else if constexpr (std::is_integral_v<U>) {
      if (is_unsigned(_var))
        return read_uint64(_var).transform(
            [](auto _v) { return static_cast<T>(_v); });
      return read_int64(_var).transform(
          [](auto _v) { return static_cast<T>(_v); });
    } else if constexpr (internal::is_literal_v<T>)
      return read_enum(_var).and_then(
          [](auto _v) { return T::from_value(_v); });
    else
      static_assert(always_false_v<T>, "Unsupported type.");
  }
  Result<InputArrayType> to_array(const InputVarType&) const noexcept;
  Result<InputObjectType> to_object(const InputVarType&) const noexcept;
  Result<InputMapType> to_map(const InputVarType&) const noexcept;
  Result<InputUnionType> to_union(const InputVarType&) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (size_t i = 0; i < array_size(_arr); ++i) {
      const auto err = _array_reader.read(array_at(_arr, i));
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
      const auto entries = map_entries(_map);
      for (size_t i = 0; i < array_size(entries); ++i) {
        const auto entry = array_at(entries, i);
        const auto key = map_key(entry);
        _map_reader.read(key, map_value(entry));
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error{e.what()};
    }
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (size_t i = 0; i < object_size(_obj); ++i) {
      _object_reader.read(i, object_at(_obj, i));
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
    const auto& [value, disc] = *opt_pair;
    return UnionReaderType::read(*this, disc, value);
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
  Result<std::string> read_string(const InputVarType&) const noexcept;
  Result<std::vector<char>> read_bytes(const InputVarType&,
                                       bool) const noexcept;
  Result<bool> read_bool(const InputVarType&) const noexcept;
  Result<double> read_double(const InputVarType&) const noexcept;
  Result<int64_t> read_int64(const InputVarType&) const noexcept;
  Result<uint64_t> read_uint64(const InputVarType&) const noexcept;
  Result<uint16_t> read_enum(const InputVarType&) const noexcept;
  bool is_unsigned(const InputVarType&) const noexcept;
  size_t array_size(const InputArrayType&) const;
  InputVarType array_at(const InputArrayType&, size_t) const;
  size_t object_size(const InputObjectType&) const;
  InputVarType object_at(const InputObjectType&, size_t) const;
  InputArrayType map_entries(const InputMapType&) const;
  std::string_view map_key(const InputVarType&) const;
  InputVarType map_value(const InputVarType&) const;
  std::optional<std::pair<InputVarType, size_t>> identify_discriminant(
      const InputUnionType& _union) const noexcept;
};

}  // namespace rfl::capnproto
#endif
