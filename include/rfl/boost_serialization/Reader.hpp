#ifndef RFL_BOOST_SERIALIZATION_READER_HPP_
#define RFL_BOOST_SERIALIZATION_READER_HPP_

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::boost_serialization {

template <class IArchive>
struct Reader {
  struct BoostInputArray {
    IArchive* ar;
    size_t size;
  };

  struct BoostInputMap {
    IArchive* ar;
    size_t size;
  };

  struct BoostInputObject {
    IArchive* ar;
    size_t size;
  };

  struct BoostInputUnion {
    IArchive* ar;
  };

  struct BoostInputVar {
    IArchive* ar;
  };

  using InputArrayType = BoostInputArray;
  using InputMapType = BoostInputMap;
  using InputObjectType = BoostInputObject;
  using InputUnionType = BoostInputUnion;
  using InputVarType = BoostInputVar;

  template <class T>
  static constexpr bool has_custom_constructor = false;

  bool is_empty(const InputVarType& /*_var*/) const noexcept { return false; }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    try {
      if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
        std::string val;
        *_var.ar >> val;
        return val;
      } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
        bool val = false;
        *_var.ar >> val;
        return val;
      } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
        double val = 0.0;
        *_var.ar >> val;
        return static_cast<T>(val);
      } else if constexpr (std::is_unsigned<std::remove_cvref_t<T>>()) {
        std::uint64_t val = 0;
        *_var.ar >> val;
        return static_cast<T>(val);
      } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
        std::int64_t val = 0;
        *_var.ar >> val;
        return static_cast<T>(val);
      } else if constexpr (internal::is_literal_v<T>) {
        std::int64_t val = 0;
        *_var.ar >> val;
        return T::from_value(
            static_cast<typename std::remove_cvref_t<T>::ValueType>(val));
      } else {
        static_assert(rfl::always_false_v<T>, "Unsupported type.");
      }
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    try {
      std::uint64_t size = 0;
      *_var.ar >> size;
      return InputArrayType{_var.ar, static_cast<size_t>(size)};
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    try {
      std::uint64_t size = 0;
      *_var.ar >> size;
      return InputObjectType{_var.ar, static_cast<size_t>(size)};
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    try {
      std::uint64_t size = 0;
      *_var.ar >> size;
      return InputMapType{_var.ar, static_cast<size_t>(size)};
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    return InputUnionType{_var.ar};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    for (size_t i = 0; i < _arr.size; ++i) {
      auto var = InputVarType{_arr.ar};
      const auto err = _array_reader.read(var);
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
      for (size_t i = 0; i < _map.size; ++i) {
        std::string key;
        *_map.ar >> key;
        auto var = InputVarType{_map.ar};
        _map_reader.read(std::string_view(key), var);
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(e.what());
    }
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    for (size_t i = 0; i < _obj.size; ++i) {
      auto var = InputVarType{_obj.ar};
      _object_reader.read(static_cast<int>(i), var);
    }
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    try {
      std::uint64_t disc = 0;
      *_union.ar >> disc;
      auto var = InputVarType{_union.ar};
      return UnionReaderType::read(*this, static_cast<size_t>(disc), var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& /*_var*/) const noexcept {
    return rfl::error("Custom constructors are not supported.");
  }
};

}  // namespace rfl::boost_serialization

#endif
