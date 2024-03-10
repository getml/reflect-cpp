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
  static constexpr const char* XML_CONTENT = "xml_content";

  struct XMLOutputArray {
    XMLOutputArray(const std::string_view& _name,
                   const Ref<pugi::xml_node>& _node)
        : name_(_name), node_(_node) {}
    std::string_view name_;
    Ref<pugi::xml_node> node_;
  };

  struct XMLOutputObject {
    XMLOutputObject(const Ref<pugi::xml_node>& _node) : node_(_node) {}
    Ref<pugi::xml_node> node_;
  };

  struct XMLOutputVar {
    XMLOutputVar(const Ref<pugi::xml_node>& _node) : node_(_node) {}
    Ref<pugi::xml_node> node_;
  };

  using OutputArrayType = XMLOutputArray;
  using OutputObjectType = XMLOutputObject;
  using OutputVarType = XMLOutputVar;

  Writer(const Ref<pugi::xml_node>& _root, const std::string& _root_name)
      : root_(_root), root_name_(_root_name) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    auto node_child =
        Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
    return OutputArrayType(root_name_, node_child);
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    auto node_child =
        Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
    return OutputObjectType(node_child);
  }

  OutputVarType null_as_root() const noexcept {
    auto node_child =
        Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
    return OutputVarType(node_child);
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    const auto str = to_string(_var);
    auto node_child =
        Ref<pugi::xml_node>::make(root_->append_child(root_name_.c_str()));
    node_child->append_child(pugi::node_pcdata).set_value(str.c_str());
    return OutputVarType(node_child);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return *_parent;
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return OutputArrayType(_name, _parent->node_);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    auto node_child = Ref<pugi::xml_node>::make(
        _parent->node_->append_child(_parent->name_.data()));
    return OutputObjectType(node_child);
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    auto node_child =
        Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
    return OutputObjectType(node_child);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    const auto str = to_string(_var);
    auto node_child = Ref<pugi::xml_node>::make(
        _parent->node_->append_child(_parent->name_.data()));
    node_child->append_child(pugi::node_pcdata).set_value(str.c_str());
    return OutputVarType(node_child);
  }

  template <class T>
  OutputVarType add_value_to_object(
      const std::string_view& _name, const T& _var, OutputObjectType* _parent,
      const bool _is_attribute = false) const noexcept {
    const auto str = to_string(_var);
    if (_is_attribute) {
      _parent->node_->append_attribute(_name.data()) = str.c_str();
      return OutputVarType(_parent->node_);
    } else if (_name == XML_CONTENT) {
      _parent->node_->append_child(pugi::node_pcdata).set_value(str.c_str());
      return OutputVarType(_parent->node_);
    } else {
      auto node_child =
          Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
      node_child->append_child(pugi::node_pcdata).set_value(str.c_str());
      return OutputVarType(node_child);
    }
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    auto node_child = Ref<pugi::xml_node>::make(
        _parent->node_->append_child(_parent->name_.data()));
    return OutputVarType(node_child);
  }

  OutputVarType add_null_to_object(
      const std::string_view& _name, OutputObjectType* _parent,
      const bool _is_attribute = false) const noexcept {
    if (_is_attribute) {
      return OutputVarType(_parent->node_);
    } else if (_name == XML_CONTENT) {
      return OutputVarType(_parent->node_);
    } else {
      auto node_child =
          Ref<pugi::xml_node>::make(_parent->node_->append_child(_name.data()));
      return OutputVarType(node_child);
    }
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  template <class T>
  std::string to_string(const T& _val) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return _val;
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return _val ? "true" : "false";
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      return std::to_string(_val);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  }

 public:
  Ref<pugi::xml_node> root_;

  std::string root_name_;
};

}  // namespace xml
}  // namespace rfl

#endif  // XML_PARSER_HPP_
