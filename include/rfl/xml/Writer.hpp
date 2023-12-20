#ifndef RFL_XML_WRITER_HPP_
#define RFL_XML_WRITER_HPP_

#include <exception>
#include <map>
#include <pugixml.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "rfl/Ref.hpp"
#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/xml/OutputArray.hpp"
#include "rfl/xml/OutputNull.hpp"
#include "rfl/xml/OutputObject.hpp"
#include "rfl/xml/OutputValue.hpp"

namespace rfl {
namespace xml {

struct Writer {
  using OutputArrayType = Ref<OutputArray>;
  using OutputObjectType = Ref<OutputObject>;
  using OutputVarType = Ref<OutputVar>;

  Writer() {}

  ~Writer() = default;

  void add(const OutputVarType _var, OutputArrayType* _arr) const noexcept {
    (*_arr)->push_back(_var);
  }

  OutputVarType empty_var() const noexcept { return make_ref<OutputNull>(); }

  template <class T>
  OutputVarType from_basic_type(
      const T& _var, const bool _is_attribute = false) const noexcept {
    return make_ref<OutputValue<T>>(_var, _is_attribute);
  }

  OutputArrayType new_array() const noexcept { return make_ref<OutputArray>(); }

  OutputObjectType new_object() const noexcept {
    return make_ref<OutputObject>();
  }

  bool is_empty(const OutputVarType& _var) const noexcept {
    return _var->is_null();
  }

  void set_field(const std::string& _name, const OutputVarType& _var,
                 OutputObjectType* _obj) const noexcept {
    (*_obj)->push_back(_name, _var);
  }
};

}  // namespace xml
}  // namespace rfl

#endif  // XML_PARSER_HPP_
