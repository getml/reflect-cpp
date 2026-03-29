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

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    try {
      _object_reader.read([&](const auto& _field) {
        (*_obj.archive_)(::cereal::make_nvp(_field.name(), _field.get()));
      });
      return std::nullopt;
    } catch (std::exception& e) {
      return Error(std::string("Cereal object read error: ") + e.what());
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

}  // namespace rfl::cereal

#endif
