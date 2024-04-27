#ifndef RFL_JSON_WRITER_HPP_
#define RFL_JSON_WRITER_HPP_

#include <yyjson.h>

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace json {

class Writer {
 public:
  struct YYJSONOutputArray {
    YYJSONOutputArray(yyjson_mut_val* _val) : val_(_val) {}
    yyjson_mut_val* val_;
  };

  struct YYJSONOutputObject {
    YYJSONOutputObject(yyjson_mut_val* _val) : val_(_val) {}
    yyjson_mut_val* val_;
  };

  struct YYJSONOutputVar {
    YYJSONOutputVar(yyjson_mut_val* _val) : val_(_val) {}

    YYJSONOutputVar(YYJSONOutputArray _arr) : val_(_arr.val_) {}

    YYJSONOutputVar(YYJSONOutputObject _obj) : val_(_obj.val_) {}

    yyjson_mut_val* val_;
  };

  using OutputArrayType = YYJSONOutputArray;
  using OutputObjectType = YYJSONOutputObject;
  using OutputVarType = YYJSONOutputVar;

  Writer(yyjson_mut_doc* _doc) : doc_(_doc) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_doc_set_root(doc_, arr);
    return OutputArrayType(arr);
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    const auto obj = yyjson_mut_obj(doc_);
    yyjson_mut_doc_set_root(doc_, obj);
    return OutputObjectType(obj);
  }

  OutputVarType null_as_root() const noexcept {
    const auto null = yyjson_mut_null(doc_);
    yyjson_mut_doc_set_root(doc_, null);
    return OutputVarType(null);
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_doc_set_root(doc_, val.val_);
    return OutputVarType(val);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_arr_add_val(_parent->val_, arr);
    return OutputArrayType(arr);
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()),
                       arr);
    return OutputArrayType(arr);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    const auto obj = yyjson_mut_obj(doc_);
    yyjson_mut_arr_add_val(_parent->val_, obj);
    return OutputObjectType(obj);
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    const auto obj = yyjson_mut_obj(doc_);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()),
                       obj);
    return OutputObjectType(obj);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_arr_add_val(_parent->val_, val.val_);
    return OutputVarType(val);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()),
                       val.val_);
    return OutputVarType(val);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    const auto null = yyjson_mut_null(doc_);
    yyjson_mut_arr_add_val(_parent->val_, null);
    return OutputVarType(null);
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    const auto null = yyjson_mut_null(doc_);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.data()),
                       null);
    return OutputVarType(null);
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  template <class T>
  OutputVarType from_basic_type(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return OutputVarType(yyjson_mut_strcpy(doc_, _var.c_str()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return OutputVarType(yyjson_mut_bool(doc_, _var));
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_real(doc_, static_cast<double>(_var)));
    } else if constexpr (std::is_unsigned<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_uint(doc_, static_cast<uint64_t>(_var)));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_int(doc_, static_cast<int64_t>(_var)));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

 public:
  yyjson_mut_doc* doc_;
};

}  // namespace json
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
