#ifndef RFL_MSGPACK_WRITER_HPP_
#define RFL_MSGPACK_WRITER_HPP_

#include <msgpack.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../always_false.hpp"
#include "../common.hpp"
#include "../concepts.hpp"

namespace rfl::msgpack {

class RFL_API Writer {
 public:
  struct MsgpackOutputArray {};

  struct MsgpackOutputObject {};

  struct MsgpackOutputVar {};

  using OutputArrayType = MsgpackOutputArray;
  using OutputObjectType = MsgpackOutputObject;
  using OutputVarType = MsgpackOutputVar;

  Writer(msgpack_packer* _pk);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t _size) const;

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

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var, OutputArrayType*) const {
    return new_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var, OutputObjectType*) const {
    write_name(_name);
    return new_value(_var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
  OutputArrayType new_array(const size_t _size) const;

  OutputObjectType new_object(const size_t _size) const;

  template <class T>
  OutputVarType new_value(const T& _var) const {
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same<Type, std::string>()) {
      return new_string(_var);
    } else if constexpr (concepts::MutableContiguousByteContainer<Type>) {
      return new_binary(_var.data(), _var.size());
    } else if constexpr (std::is_same<Type, bool>()) {
      return new_bool(_var);
    } else if constexpr (std::is_floating_point<Type>()) {
      return new_double(static_cast<double>(_var));
    } else if constexpr (std::is_unsigned<Type>()) {
      return new_uint64(static_cast<std::uint64_t>(_var));
    } else if constexpr (std::is_integral<Type>()) {
      return new_int64(static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  // Keep calls to MessagePack's static inline functions out of exported templates.
  void write_name(const std::string_view& _name) const;

  OutputVarType new_string(const std::string_view& _value) const;
  OutputVarType new_binary(const void* _data, size_t _size) const;
  OutputVarType new_bool(bool _value) const;
  OutputVarType new_double(double _value) const;
  OutputVarType new_uint64(std::uint64_t _value) const;
  OutputVarType new_int64(std::int64_t _value) const;

 private:
  /// The underlying packer.
  msgpack_packer* pk_;
};

}  // namespace rfl::msgpack

#endif
