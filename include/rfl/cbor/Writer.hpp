#ifndef RFL_CBOR_WRITER_HPP_
#define RFL_CBOR_WRITER_HPP_

#include <bit>
#include <exception>
#include <jsoncons_ext/cbor/cbor_encoder.hpp>
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

namespace rfl::cbor {

class Writer {
  using Encoder = jsoncons::cbor::cbor_bytes_encoder;

 public:
  struct CBOROutputArray {};

  struct CBOROutputObject {};

  struct CBOROutputVar {};

  using OutputArrayType = CBOROutputArray;
  using OutputObjectType = CBOROutputObject;
  using OutputVarType = CBOROutputVar;

  Writer(Encoder* _encoder);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return new_value(_var);
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
    return new_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    encoder_->key(_name);
    return new_value(_var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
  OutputArrayType new_array(const size_t _size) const noexcept;

  OutputObjectType new_object(const size_t _size) const noexcept;

  template <class T>
  OutputVarType new_value(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      encoder_->string_value(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      encoder_->byte_string_value(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      encoder_->bool_value(_var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      encoder_->double_value(static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      encoder_->int64_value(static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

 private:
  /// The underlying TinyCBOR encoder.
  Encoder* const encoder_;
};
}  // namespace rfl::cbor

#endif
