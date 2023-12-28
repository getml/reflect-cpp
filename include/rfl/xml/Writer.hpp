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

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace xml {

struct Writer {
  struct XMLOutputArray {
    XMLOutputArray(const pugi::xml_node& _val) : val_(_val) {}
    pugi::xml_node val_;
  };

  struct XMLOutputObject {
    XMLOutputObject(const pugi::xml_node& _val) : val_(_val) {}
    pugi::xml_node val_;
  };

  struct XMLOutputVar {
    XMLOutputVar(const pugi::xml_node& _val) : val_(_val) {}
    pugi::xml_node val_;
  };

  using OutputArrayType = XMLOutputArray;
  using OutputObjectType = XMLOutputObject;
  using OutputVarType = XMLOutputVar;

  Writer(const pugi::xml_node& _root) : root_(_root) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    return OutputArrayType(root_);
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return OutputObjectType(root_);
  }

  OutputVarType null_as_root() const noexcept { return OutputVarType(root_); }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    const auto str = to_string(_var);
    root_.append_child(pugi::node_pcdata).set_value(str.c_str());
    return OutputVarType(root_);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return OutputArrayType(_parent->val_);
  }

 private:
  template <class T>
  std::string to_string(const T& _val) const noexcept {
    if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
      return _val;
    } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
      return _val ? "true" : "false";
    } else if constexpr (std::is_floating_point<std::decay_t<T>>() ||
                         std::is_integral<std::decay_t<T>>()) {
      return std::to_string(_val);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  }

 public:
  pugi::xml_node root_;
};

}  // namespace xml
}  // namespace rfl

#endif  // XML_PARSER_HPP_
