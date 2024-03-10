#ifndef RFL_CBOR_WRITER_HPP_
#define RFL_CBOR_WRITER_HPP_

#include <cbor.h>

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Box.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace cbor {

class Writer {
 public:
  struct CBOROutputArray {
    CborEncoder* encoder_;
    CborEncoder* parent_;
  };

  struct CBOROutputObject {
    CborEncoder* encoder_;
    CborEncoder* parent_;
  };

  struct CBOROutputVar {};

  using OutputArrayType = CBOROutputArray;
  using OutputObjectType = CBOROutputObject;
  using OutputVarType = CBOROutputVar;

  Writer(CborEncoder* _encoder) : encoder_(_encoder) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    return new_array(_size, encoder_);
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return new_object(_size, encoder_);
  }

  OutputVarType null_as_root() const noexcept {
    cbor_encode_null(encoder_);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return new_value(_var, encoder_);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return new_array(_size, _parent->encoder_);
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
    return new_array(_size, _parent->encoder_);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    return new_object(_size, _parent->encoder_);
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
    return new_object(_size, _parent->encoder_);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    return new_value(_var, _parent->encoder_);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
    return new_value(_var, _parent->encoder_);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    cbor_encode_null(_parent->encoder_);
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    cbor_encode_text_string(_parent->encoder_, _name.data(), _name.size());
    cbor_encode_null(_parent->encoder_);
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {
    cbor_encoder_close_container(_arr->parent_, _arr->encoder_);
  }

  void end_object(OutputObjectType* _obj) const noexcept {
    cbor_encoder_close_container(_obj->parent_, _obj->encoder_);
  }

 private:
  OutputArrayType new_array(const size_t _size,
                            CborEncoder* _parent) const noexcept {
    subencoders_->emplace_back(rfl::Box<CborEncoder>::make());
    cbor_encoder_create_array(_parent, subencoders_->back().get(), _size);
    return OutputArrayType{subencoders_->back().get(), _parent};
  }

  OutputObjectType new_object(const size_t _size,
                              CborEncoder* _parent) const noexcept {
    subencoders_->emplace_back(rfl::Box<CborEncoder>::make());
    cbor_encoder_create_map(_parent, subencoders_->back().get(), _size);
    return OutputObjectType{subencoders_->back().get(), _parent};
  }

  template <class T>
  OutputVarType new_value(const T& _var, CborEncoder* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      cbor_encode_text_string(_parent, _var.c_str(), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      cbor_encode_boolean(_parent, _var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      cbor_encode_double(_parent, static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      cbor_encode_int(_parent, static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

 private:
  /// The underlying TinyCBOR encoder.
  CborEncoder* const encoder_;

  /// Contain all of the subobjects and subarrays.
  const rfl::Box<std::vector<rfl::Box<CborEncoder>>> subencoders_;
};

}  // namespace cbor
}  // namespace rfl

#endif  // CBOR_PARSER_HPP_
