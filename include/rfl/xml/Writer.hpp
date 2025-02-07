#ifndef RFL_XML_WRITER_HPP_
#define RFL_XML_WRITER_HPP_

#include <pugixml.hpp>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Ref.hpp"
#include "../always_false.hpp"

namespace rfl {
namespace xml {

struct Writer {
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

  Writer(const Ref<pugi::xml_node>& _root, const std::string& _root_name);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    const auto str = to_string(_var);
    return value_as_root_impl(str);
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
    const auto str = to_string(_var);
    return add_value_to_array_impl(str, _parent);
  }

  template <class T>
  OutputVarType add_value_to_object(
      const std::string_view& _name, const T& _var, OutputObjectType* _parent,
      const bool _is_attribute = false) const noexcept {
    const auto str = to_string(_var);
    return add_value_to_object_impl(_name, str, _parent, _is_attribute);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_object(
      const std::string_view& _name, OutputObjectType* _parent,
      const bool _is_attribute = false) const noexcept;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

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

  OutputVarType value_as_root_impl(const std::string& _str) const noexcept;

  OutputVarType add_value_to_array_impl(
      const std::string& _str, OutputArrayType* _parent) const noexcept;

  OutputVarType add_value_to_object_impl(
      const std::string_view& _name, const std::string& _str,
      OutputObjectType* _parent,
      const bool _is_attribute = false) const noexcept;

 public:
  Ref<pugi::xml_node> root_;

  std::string root_name_;
};

}  // namespace xml
}  // namespace rfl

#endif  // RFL_XML_WRITER_HPP_
