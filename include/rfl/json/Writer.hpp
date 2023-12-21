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

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"

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

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_arr_add_val(_parent->val_, arr);
    return OutputArrayType(arr);
  }

  OutputArrayType add_array_to_object(
      const std::string& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.c_str()),
                       arr);
    return OutputArrayType(arr);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_arr_add_val(_parent->val_, arr);
    return OutputArrayType(arr);
  }

  OutputArrayType add_array_to_object(
      const std::string& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    const auto arr = yyjson_mut_arr(doc_);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.c_str()),
                       arr);
    return OutputArrayType(arr);
  }
  template <class T>
  void add_value_to_array(const T& _var,
                          OutputArrayType* _parent) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_arr_add_val(_parent->val_, val);
  }

  template <class T>
  void add_value_to_object(const std::string& _name, const T& _var,
                           OutputObjectType* _parent) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_obj_add(_parent->val_, yyjson_mut_strcpy(doc_, _name.c_str()),
                       val);
  }

  OutputVarType empty_var() const noexcept {
    return OutputVarType(yyjson_mut_null(doc_));
  }

  bool is_empty(const OutputVarType& _var) const noexcept {
    return yyjson_mut_is_null(_var.val_);
  }

 private:
  template <class T>
  OutputVarType from_basic_type(const T& _var) const noexcept {
    if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
      return OutputVarType(yyjson_mut_strcpy(doc_, _var.c_str()));
    } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
      return OutputVarType(yyjson_mut_bool(doc_, _var));
    } else if constexpr (std::is_floating_point<std::decay_t<T>>()) {
      return OutputVarType(yyjson_mut_real(doc_, static_cast<double>(_var)));
    } else if constexpr (std::is_integral<std::decay_t<T>>()) {
      return OutputVarType(yyjson_mut_int(doc_, static_cast<int>(_var)));
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
