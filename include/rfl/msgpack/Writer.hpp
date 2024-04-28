#ifndef RFL_MSGPACK_WRITER_HPP_
#define RFL_MSGPACK_WRITER_HPP_

#include <msgpack.h>

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

namespace rfl::msgpack {

class Writer {
 public:
  struct MsgpackOutputArray {};

  struct MsgpackOutputObject {};

  struct MsgpackOutputVar {};

  using OutputArrayType = MsgpackOutputArray;
  using OutputObjectType = MsgpackOutputObject;
  using OutputVarType = MsgpackOutputVar;

  Writer(msgpack_packer* _pk) : pk_(_pk) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    return new_array(_size);
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return new_object(_size);
  }

  OutputVarType null_as_root() const noexcept {
    msgpack_pack_nil(pk_);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return new_value(_var);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return new_array(_size);
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    msgpack_pack_str(pk_, _name.size());
    msgpack_pack_str_body(pk_, _name.data(), _name.size());
    return new_array(_size);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    return new_object(_size);
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    msgpack_pack_str(pk_, _name.size());
    msgpack_pack_str_body(pk_, _name.data(), _name.size());
    return new_object(_size);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    return new_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    msgpack_pack_str(pk_, _name.size());
    msgpack_pack_str_body(pk_, _name.data(), _name.size());
    return new_value(_var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    msgpack_pack_nil(pk_);
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    msgpack_pack_str(pk_, _name.size());
    msgpack_pack_str_body(pk_, _name.data(), _name.size());
    msgpack_pack_nil(pk_);
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  OutputArrayType new_array(const size_t _size) const noexcept {
    msgpack_pack_array(pk_, _size);
    return OutputArrayType{};
  }

  OutputObjectType new_object(const size_t _size) const noexcept {
    msgpack_pack_map(pk_, _size);
    return OutputObjectType{};
  }

  template <class T>
  OutputVarType new_value(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      msgpack_pack_str(pk_, _var.size());
      msgpack_pack_str_body(pk_, _var.c_str(), _var.size());
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      if (_var) {
        msgpack_pack_true(pk_);
      } else {
        msgpack_pack_false(pk_);
      }
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      msgpack_pack_double(pk_, static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      msgpack_pack_int64(pk_, static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

 private:
  /// The underlying packer.
  msgpack_packer* pk_;
};

}  // namespace rfl::msgpack

#endif  // MSGPACK_PARSER_HPP_
