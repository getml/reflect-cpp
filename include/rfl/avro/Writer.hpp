#ifndef RFL_AVRO_WRITER_HPP_
#define RFL_AVRO_WRITER_HPP_

#include <avro.h>

#include <bit>
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
#include "../Bytestring.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace avro {

class Writer {
 public:
  struct AVROOutputArray {
    AvroEncoder* encoder_;
    AvroEncoder* parent_;
  };

  struct AVROOutputObject {
    AvroEncoder* encoder_;
    AvroEncoder* parent_;
  };

  struct AVROOutputVar {};

  using OutputArrayType = AVROOutputArray;
  using OutputObjectType = AVROOutputObject;
  using OutputVarType = AVROOutputVar;

  Writer(AvroEncoder* _encoder);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return new_value(_var, encoder_);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    return new_value(_var, _parent->encoder_);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    avro_encode_text_string(_parent->encoder_, _name.data(), _name.size());
    return new_value(_var, _parent->encoder_);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
  OutputArrayType new_array(const size_t _size,
                            AvroEncoder* _parent) const noexcept;

  OutputObjectType new_object(const size_t _size,
                              AvroEncoder* _parent) const noexcept;

  template <class T>
  OutputVarType new_value(const T& _var, AvroEncoder* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      avro_encode_text_string(_parent, _var.c_str(), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      avro_encode_byte_string(
          _parent, std::bit_cast<const uint8_t*>(_var.c_str()), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      avro_encode_boolean(_parent, _var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      avro_encode_double(_parent, static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      avro_encode_int(_parent, static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

 private:
  /// The underlying TinyAVRO encoder.
  AvroEncoder* const encoder_;

  /// Contain all of the subobjects and subarrays.
  const rfl::Box<std::vector<rfl::Box<AvroEncoder>>> subencoders_;
};

}  // namespace avro
}  // namespace rfl

#endif
