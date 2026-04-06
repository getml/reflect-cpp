#ifndef RFL_YAS_READER_HPP_
#define RFL_YAS_READER_HPP_

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <yas/binary_iarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"
#include "../parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::yas {

struct Reader {
  using IArchive = ::yas::binary_iarchive<::yas::mem_istream>;

  struct InputVarType {
    IArchive* ar;
  };

  struct InputArrayType {
    IArchive* ar;
  };

  struct InputObjectType {
    IArchive* ar;
  };

  struct InputMapType {
    IArchive* ar;
  };

  struct InputUnionType {
    IArchive* ar;
  };

  template <class T>
  static constexpr bool has_custom_constructor = false;

  bool is_empty(const InputVarType& /*_var*/) const noexcept { return false; }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    try {
      if constexpr (internal::is_literal_v<T>) {
        std::string str;
        (*_var.ar) & str;
        return std::remove_cvref_t<T>::from_string(str);
      } else {
        T value;
        (*_var.ar) & value;
        return value;
      }
    } catch (std::exception& e) {
      return error(std::string("yas read error: ") + e.what());
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    return InputArrayType{_var.ar};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    return InputObjectType{_var.ar};
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    return InputMapType{_var.ar};
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    return InputUnionType{_var.ar};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    try {
      std::size_t size;
      (*_arr.ar) & size;
      for (std::size_t i = 0; i < size; ++i) {
        const auto err = _array_reader.read(InputVarType{_arr.ar});
        if (err) {
          return err;
        }
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("yas array read error: ") + e.what());
    }
  }

  template <class MapReader>
  std::optional<Error> read_map(const MapReader& _map_reader,
                                const InputMapType& _map) const noexcept {
    try {
      std::size_t size;
      (*_map.ar) & size;
      for (std::size_t i = 0; i < size; ++i) {
        std::string key;
        (*_map.ar) & key;
        _map_reader.read(std::string_view(key), InputVarType{_map.ar});
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("yas map read error: ") + e.what());
    }
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    try {
      for (std::size_t i = 0; i < ObjectReader::size(); ++i) {
        _object_reader.read(i, InputVarType{_obj.ar});
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("yas object read error: ") + e.what());
    }
  }

  template <class T, class UnionReader>
  rfl::Result<T> read_union(const InputUnionType& _union) const noexcept {
    try {
      std::size_t index;
      (*_union.ar) & index;
      return UnionReader::read(*this, index, InputVarType{_union.ar});
    } catch (std::exception& e) {
      return error(std::string("yas union read error: ") + e.what());
    }
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& /*_var*/) const noexcept {
    return rfl::error("Custom constructors are not supported.");
  }
};

static_assert(parsing::schemaful::IsSchemafulReader<Reader>);

}  // namespace rfl::yas

#endif
