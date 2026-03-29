#ifndef RFL_CEREAL_READER_HPP_
#define RFL_CEREAL_READER_HPP_

#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <exception>
#include <sstream>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::cereal {

struct Reader {
  using CerealArchive = ::cereal::PortableBinaryInputArchive;

  struct CerealInputVar {
    CerealArchive* archive_;
  };

  struct InputVarType {
    CerealArchive* archive_;
  };

  struct InputArrayType {
    CerealArchive* archive_;
  };

  struct InputObjectType {
    CerealArchive* archive_;
  };

  struct InputMapType {
    CerealArchive* archive_;
  };

  struct InputUnionType {
    CerealArchive* archive_;
  };

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_cereal_obj(var); });

  bool is_empty(const InputVarType& _var) const noexcept {
    return _var.archive_ == nullptr;
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    try {
      T value;
      (*_var.archive_)(value);
      return value;
    } catch (std::exception& e) {
      return error(std::string("Cereal read error: ") + e.what());
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    return InputArrayType{_var.archive_};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    return InputObjectType{_var.archive_};
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    return InputMapType{_var.archive_};
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    return InputUnionType{_var.archive_};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    try {
      ::cereal::size_type size;
      (*_arr.archive_)(::cereal::make_size_tag(size));
      for (::cereal::size_type i = 0; i < size; ++i) {
        const auto err = _array_reader.read(InputVarType{_arr.archive_});
        if (err) {
          return err;
        }
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("Cereal array read error: ") + e.what());
    }
  }

  template <class MapReader>
  std::optional<Error> read_map(const MapReader& _map_reader,
                                const InputMapType& _map) const noexcept {
    try {
      ::cereal::size_type size;
      (*_map.archive_)(::cereal::make_size_tag(size));
      for (::cereal::size_type i = 0; i < size; ++i) {
        std::string key;
        (*_map.archive_)(key);
        const auto err = _map_reader.read(std::string_view(key),
                                          InputVarType{_map.archive_});
        if (err) {
          return err;
        }
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("Cereal map read error: ") + e.what());
    }
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    try {
      for (size_t i = 0; i < ObjectReader::size(); ++i) {
        _object_reader.read(i, InputVarType{_obj.archive_});
      }
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("Cereal object read error: ") + e.what());
    }
  }

  template <class T, class UnionReader>
  rfl::Result<T> read_union(const InputUnionType& _union) const noexcept {
    try {
      std::int32_t index;
      (*_union.archive_)(index);
      return UnionReader::read(*this, index, InputVarType{_union.archive_});
    } catch (std::exception& e) {
      return error(std::string("Cereal union read error: ") + e.what());
    }
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_cereal_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

static_assert(parsing::schemaful::IsSchemafulReader<Reader>);

}  // namespace rfl::cereal

#endif
