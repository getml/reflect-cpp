#ifndef RFL_CBOR_WRITER_HPP_
#define RFL_CBOR_WRITER_HPP_

#include <jsoncons_ext/cbor/cbor_encoder.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Bytestring.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl::cbor {

class RFL_API Writer {
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

  OutputArrayType array_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t) const;

  OutputVarType null_as_root() const;

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    return new_value(_var);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  OutputObjectType add_object_to_array(const size_t,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t,
                                        OutputObjectType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* /*_parent*/) const {
    return new_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* /*_parent*/) const {
    encoder_->key(_name);
    return new_value(_var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  void end_array(OutputArrayType* _arr) const;

  void end_object(OutputObjectType* _obj) const;

 private:
  OutputArrayType new_array(const size_t _size) const;

  OutputObjectType new_object() const;

  template <class T>
  OutputVarType new_value(const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      encoder_->string_value(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>() ||
                         std::is_same<std::remove_cvref_t<T>,
                                      rfl::Vectorstring>()) {
      encoder_->byte_string_value(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      encoder_->bool_value(_var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      encoder_->double_value(static_cast<double>(_var));
    } else if constexpr (std::is_unsigned<std::remove_cvref_t<T>>()) {
      encoder_->uint64_value(static_cast<std::uint64_t>(_var));
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
