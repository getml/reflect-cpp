#ifndef RFL_JSON_WRITER_HPP_
#define RFL_JSON_WRITER_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

// #include "../Result.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl {
namespace json {

class RFL_API Writer {
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

  Writer();

  ~Writer() = default;

  yyjson_mut_doc* doc() const { return doc_.get(); }

  OutputArrayType array_as_root(const size_t) const noexcept;

  OutputObjectType object_as_root(const size_t) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    const auto val = from_basic_type(_var);
    yyjson_mut_doc_set_root(doc(), val.val_);
    return OutputVarType(val);
  }

  OutputArrayType add_array_to_array(const size_t,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t,
                                      OutputObjectType* _parent) const;

  OutputObjectType add_object_to_array(const size_t,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t,
                                        OutputObjectType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    const auto val = from_basic_type(_var);
    const bool ok = yyjson_mut_arr_add_val(_parent->val_, val.val_);
    if (!ok) {
      throw std::runtime_error("Could not add value to array.");
    }
    return OutputVarType(val);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    const auto val = from_basic_type(_var);
    const bool ok = yyjson_mut_obj_add(
        _parent->val_, yyjson_mut_strcpy(doc(), _name.data()), val.val_);
    if (!ok) {
      throw std::runtime_error("Could not add field '" + std::string(_name) +
                               "' to object.");
    }
    return OutputVarType(val);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  void end_array(OutputArrayType*) const noexcept;

  void end_object(OutputObjectType*) const noexcept;

 private:
  template <class T>
  OutputVarType from_basic_type(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return OutputVarType(yyjson_mut_strcpy(doc(), _var.c_str()));

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return OutputVarType(yyjson_mut_bool(doc(), _var));

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_real(doc(), static_cast<double>(_var)));

    } else if constexpr (std::is_unsigned<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_uint(doc(), static_cast<uint64_t>(_var)));

    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return OutputVarType(yyjson_mut_int(doc(), static_cast<int64_t>(_var)));

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

 private:
  std::shared_ptr<yyjson_mut_doc> doc_;
};

}  // namespace json
}  // namespace rfl

#endif
