#ifndef FLEXBUF_WRITER_HPP_
#define FLEXBUF_WRITER_HPP_

#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <exception>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace flexbuf {

struct Writer {
  struct OutputArray {
    size_t start_;
  };

  struct OutputObject {
    size_t start_;
  };

  struct OutputVar {};

  using OutputArrayType = OutputArray;
  using OutputObjectType = OutputObject;
  using OutputVarType = OutputVar;

  Writer(const Ref<flexbuffers::Builder>& _fbb) : fbb_(_fbb) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    return new_array();
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return new_object();
  }

  OutputVarType null_as_root() const noexcept {
    fbb_->Null();
    return OutputVarType{};
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return insert_value(_var);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return new_array();
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return new_array(_name);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    return new_object();
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return new_object(_name);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    return insert_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    return insert_value(_name, _var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    fbb_->Null();
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    fbb_->Null(_name.data());
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {
    fbb_->EndVector(_arr->start_, false, false);
  }

  void end_object(OutputObjectType* _obj) const noexcept {
    fbb_->EndMap(_obj->start_);
  }

 private:
  template <class T>
  OutputVarType insert_value(const std::string_view& _name,
                             const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      fbb_->String(_name.data(), _var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      fbb_->Blob(_name.data(), _var.c_str(), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      fbb_->Bool(_name.data(), _var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      fbb_->Double(_name.data(), _var);
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      fbb_->Int(_name.data(), _var);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType insert_value(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      fbb_->String(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      fbb_->Blob(_var.c_str(), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      fbb_->Bool(_var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      fbb_->Double(_var);
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      fbb_->Int(_var);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
    return OutputVarType{};
  }

  OutputArrayType new_array(const std::string_view& _name) const noexcept {
    const auto start = fbb_->StartVector(_name.data());
    return OutputArrayType{start};
  }

  OutputArrayType new_array() const noexcept {
    const auto start = fbb_->StartVector();
    return OutputArrayType{start};
  }

  OutputObjectType new_object(const std::string_view& _name) const noexcept {
    const auto start = fbb_->StartMap(_name.data());
    return OutputObjectType{start};
  }

  OutputObjectType new_object() const noexcept {
    const auto start = fbb_->StartMap();
    return OutputObjectType{start};
  }

 private:
  Ref<flexbuffers::Builder> fbb_;
};

}  // namespace flexbuf
}  // namespace rfl

#endif
