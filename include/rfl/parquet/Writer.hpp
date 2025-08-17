#ifndef RFL_MSGPACK_WRITER_HPP_
#define RFL_MSGPACK_WRITER_HPP_

#include <parquet.h>

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
#include "../Vectorstring.hpp"
#include "../always_false.hpp"

namespace rfl::parquet {

class Writer {
 public:
  struct ParquetOutputArray {};

  struct ParquetOutputObject {};

  struct ParquetOutputVar {};

  using OutputArrayType = ParquetOutputArray;
  using OutputObjectType = ParquetOutputObject;
  using OutputVarType = ParquetOutputVar;

  Writer(parquet_packer* _pk);

  ~Writer();

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
    parquet_pack_str(pk_, _name.size());
    parquet_pack_str_body(pk_, _name.data(), _name.size());
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
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same<Type, std::string>()) {
      parquet_pack_str(pk_, _var.size());
      parquet_pack_str_body(pk_, _var.c_str(), _var.size());
    } else if constexpr (std::is_same<Type, rfl::Bytestring>() ||
                         std::is_same<Type, rfl::Vectorstring>()) {
      parquet_pack_bin(pk_, _var.size());
      parquet_pack_bin_body(pk_, _var.data(), _var.size());
    } else if constexpr (std::is_same<Type, bool>()) {
      if (_var) {
        parquet_pack_true(pk_);
      } else {
        parquet_pack_false(pk_);
      }
    } else if constexpr (std::is_floating_point<Type>()) {
      parquet_pack_double(pk_, static_cast<double>(_var));
    } else if constexpr (std::is_integral<Type>()) {
      parquet_pack_int64(pk_, static_cast<std::int64_t>(_var));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

 private:
  /// The underlying packer.
  parquet_packer* pk_;
};

}  // namespace rfl::parquet

#endif
