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

struct Writer {
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

  void add(const OutputVarType _var, OutputArrayType* _arr) const noexcept {
    yyjson_mut_arr_add_val(_arr->val_, _var.val_);
  }

  OutputVarType empty_var() const noexcept {
    return OutputVarType(yyjson_mut_null(doc_));
  }

  template <class T>
  OutputVarType from_basic_type(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return OutputVarType(yyjson_mut_strcpy(doc_, _var.c_str()));
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return OutputVarType(yyjson_mut_bool(doc_, _var));
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_real(doc_, static_cast<double>(_var)));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_int(doc_, static_cast<int>(_var)));
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  OutputArrayType new_array() const noexcept {
    return OutputArrayType(yyjson_mut_arr(doc_));
  }

  OutputObjectType new_object() const noexcept {
    return OutputObjectType(yyjson_mut_obj(doc_));
  }

  bool is_empty(const OutputVarType& _var) const noexcept {
    return yyjson_mut_is_null(_var.val_);
  }

  void set_field(const std::string& _name, const OutputVarType& _var,
                 OutputObjectType* _obj) const noexcept {
    yyjson_mut_obj_add(_obj->val_, yyjson_mut_strcpy(doc_, _name.c_str()),
                       _var.val_);
  }

  yyjson_mut_doc* doc_;
};

}  // namespace json
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
