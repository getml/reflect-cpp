#ifndef RFL_BSON_WRITER_HPP_
#define RFL_BSON_WRITER_HPP_

#include <bson/bson.h>

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

  Writer(bson_t* _doc) : doc_(_doc) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    bson_array_builder_t* val = bson_array_builder_new();
    return OutputArrayType(val, IsRoot{});
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return OutputObjectType(doc_, IsRoot{});
  }

  OutputVarType null_as_root() const noexcept {
    // Appears to be unsupported by the BSON C API.
    return OutputVarType{};
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    static_assert(rfl::always_false_v<T>,
                  "BSON only allows arrays or objects as its root.");
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    bson_array_builder_t* val;
    bson_array_builder_append_array_builder_begin(_parent->val_, &val);
    return OutputArrayType(val, IsArray{_parent->val_});
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    bson_array_builder_t* val;
    bson_append_array_builder_begin(_parent->val_, _name.data(),
                                    static_cast<int>(_name.size()), &val);
    return OutputArrayType(val, IsObject{_parent->val_});
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    subdocs_->emplace_back(rfl::Box<BSONType>());
    bson_array_builder_append_document_begin(_parent->val_,
                                             &(subdocs_->back()->val_));
    return OutputObjectType(&subdocs_->back()->val_, IsArray{_parent->val_});
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    subdocs_->emplace_back(rfl::Box<BSONType>());
    bson_append_document_begin(_parent->val_, _name.data(),
                               static_cast<int>(_name.size()),
                               &(subdocs_->back()->val_));
    return OutputObjectType(&subdocs_->back()->val_, IsObject{_parent->val_});
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      bson_array_builder_append_utf8(_parent->val_, _var.c_str(),
                                     static_cast<int>(_var.size()));
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

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    bson_array_builder_append_null(_parent->val_);
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    bson_append_null(_parent->val_, _name.data(),
                     static_cast<int>(_name.size()));
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {
    const auto handle = [&](const auto _parent) {
      using Type = std::remove_cvref_t<decltype(_parent)>;
      if constexpr (std::is_same<Type, IsArray>()) {
        bson_array_builder_append_array_builder_end(_parent.ptr_, _arr->val_);
      } else if constexpr (std::is_same<Type, IsObject>()) {
        bson_append_array_builder_end(_parent.ptr_, _arr->val_);
      } else if constexpr (std::is_same<Type, IsRoot>()) {
        bson_array_builder_build(_arr->val_, doc_);
      } else {
        static_assert(rfl::always_false_v<Type>, "Unsupported type.");
      }
    };
    std::visit(handle, _arr->parent_);
  }

  void end_object(OutputObjectType* _obj) const noexcept {
    const auto handle = [&](const auto _parent) {
      using Type = std::remove_cvref_t<decltype(_parent)>;
      if constexpr (std::is_same<Type, IsArray>()) {
        bson_array_builder_append_document_end(_parent.ptr_, _obj->val_);
      } else if constexpr (std::is_same<Type, IsObject>()) {
        bson_append_document_end(_parent.ptr_, _obj->val_);
      } else if constexpr (std::is_same<Type, IsRoot>()) {
      } else {
        static_assert(rfl::always_false_v<Type>, "Unsupported type.");
      }
    };
    std::visit(handle, _obj->parent_);
  }

 private:
  /// Pointer to the main document. In BSON, documents are what are usually
  /// called objects.
  bson_t* const doc_;

  /// Contain all of the subdocuments.
  const rfl::Ref<std::vector<rfl::Box<BSONType>>> subdocs_;
};

}  // namespace bson
}  // namespace rfl

#endif  // BSON_PARSER_HPP_
