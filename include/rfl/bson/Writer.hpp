#ifndef RFL_BSON_WRITER_HPP_
#define RFL_BSON_WRITER_HPP_

#include <bson/bson.h>

#include <cstddef>
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
#include "../internal/ptr_cast.hpp"

namespace rfl {
namespace bson {

/// Please refer to https://mongoc.org/libbson/current/api.html
class Writer {
  struct BSONType {
    bson_t val_;
  };

  struct IsArray {
    bson_array_builder_t* ptr_;
  };

  struct IsObject {
    bson_t* ptr_;
  };

  struct IsRoot {};

  using ParentType = std::variant<IsArray, IsObject, IsRoot>;

 public:
  struct BSONOutputArray {
    BSONOutputArray(bson_array_builder_t* _val, ParentType _parent)
        : parent_(_parent), val_(_val) {}
    ParentType parent_;
    bson_array_builder_t* val_;
  };

  struct BSONOutputObject {
    BSONOutputObject(bson_t* _val, ParentType _parent)
        : parent_(_parent), val_(_val) {}
    ParentType parent_;
    bson_t* val_;
  };

  struct BSONOutputVar {};

  using OutputArrayType = BSONOutputArray;
  using OutputObjectType = BSONOutputObject;
  using OutputVarType = BSONOutputVar;

  Writer(bson_t* _doc);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    static_assert(rfl::always_false_v<T>,
                  "BSON only allows arrays or objects as its root.");
    return OutputVarType{};
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
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      bson_array_builder_append_utf8(_parent->val_, _var.c_str(),
                                     static_cast<int>(_var.size()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      bson_array_builder_append_binary(
          _parent->val_, BSON_SUBTYPE_BINARY,
          internal::ptr_cast<const uint8_t*>(_var.data()),
          static_cast<uint32_t>(_var.size()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      bson_array_builder_append_bool(_parent->val_, _var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      bson_array_builder_append_double(_parent->val_,
                                       static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      bson_array_builder_append_int64(_parent->val_,
                                      static_cast<std::int64_t>(_var));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bson_oid_t>()) {
      bson_array_builder_append_oid(_parent->val_, &_var);
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      bson_append_utf8(_parent->val_, _name.data(),
                       static_cast<int>(_name.size()), _var.c_str(),
                       static_cast<int>(_var.size()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                      rfl::Bytestring>()) {
      bson_append_binary(_parent->val_, _name.data(),
                         static_cast<int>(_name.size()), BSON_SUBTYPE_BINARY,
                         internal::ptr_cast<const uint8_t*>(_var.data()),
                         static_cast<uint32_t>(_var.size()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      bson_append_bool(_parent->val_, _name.data(),
                       static_cast<int>(_name.size()), _var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      bson_append_double(_parent->val_, _name.data(),
                         static_cast<int>(_name.size()),
                         static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      bson_append_int64(_parent->val_, _name.data(),
                        static_cast<int>(_name.size()),
                        static_cast<std::int64_t>(_var));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bson_oid_t>()) {
      bson_append_oid(_parent->val_, _name.data(),
                      static_cast<int>(_name.size()), &_var);
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
  /// Pointer to the main document. In BSON, documents are what are usually
  /// called objects.
  bson_t* const doc_;

  /// Contain all of the subdocuments.
  const rfl::Ref<std::vector<rfl::Box<BSONType>>> subdocs_;
};

}  // namespace bson
}  // namespace rfl

#endif
