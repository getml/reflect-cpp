#ifndef FLEXBUF_WRITER_HPP_
#define FLEXBUF_WRITER_HPP_

#include <flatbuffers/flexbuffers.h>

#include <exception>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "rfl/Ref.hpp"
#include "rfl/Result.hpp"
#include "rfl/flexbuf/OutputArray.hpp"
#include "rfl/flexbuf/OutputNull.hpp"
#include "rfl/flexbuf/OutputObject.hpp"
#include "rfl/flexbuf/OutputValue.hpp"
#include "rfl/flexbuf/OutputVar.hpp"

namespace rfl {
namespace flexbuf {

struct Writer {
  using OutputArrayType = Ref<OutputArray>;
  using OutputObjectType = Ref<OutputObject>;
  using OutputVarType = Ref<OutputVar>;

  Writer() {}

  ~Writer() = default;

  void add(const OutputVarType& _var, OutputArrayType* _arr) const noexcept {
    (*_arr)->push_back(_var);
  }

  OutputVarType empty_var() const noexcept { return Ref<OutputNull>::make(); }

  template <class T>
  OutputVarType from_basic_type(const T& _var) const noexcept {
    return Ref<OutputValue<T>>::make(_var);
  }

  OutputArrayType new_array() const noexcept {
    return Ref<OutputArray>::make();
  }

  OutputObjectType new_object() const noexcept {
    return Ref<OutputObject>::make();
  }

  bool is_empty(const OutputVarType& _var) const noexcept {
    return _var->is_null();
  }

  void set_field(const std::string& _name, const OutputVarType& _var,
                 OutputObjectType* _obj) const noexcept {
    (*_obj)->push_back(_name, _var);
  }
};

}  // namespace flexbuf
}  // namespace rfl

#endif
