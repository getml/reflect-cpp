#ifndef RFL_XML_WRITER_HPP_
#define RFL_XML_WRITER_HPP_

#include <pugixml.hpp>
#include <string>
#include <string_view>
#include <type_traits>

#include "../Ref.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl::xml {

/// Writer class for serializing C++ objects to XML (Extensible Markup Language) format.
/// This class provides the interface for converting C++ objects into XML text.
/// Uses the pugixml library for XML generation. XML supports both elements (nodes)
/// and attributes, providing flexibility in how data is represented.
struct RFL_API Writer {
  /// Represents an XML array being constructed during serialization.
  /// In XML, arrays are represented as multiple sibling elements with the same name.
  /// Stores the element name and parent node where array elements will be added.
  struct XMLOutputArray {
    XMLOutputArray(const std::string_view& _name,
                   const Ref<pugi::xml_node>& _node)
        : name_(_name), node_(_node) {}
    std::string_view name_;
    Ref<pugi::xml_node> node_;
  };

  /// Represents an XML object being constructed during serialization.
  /// Wraps a reference to the pugixml node that will contain child elements and attributes.
  struct XMLOutputObject {
    XMLOutputObject(const Ref<pugi::xml_node>& _node) : node_(_node) {}
    Ref<pugi::xml_node> node_;
  };

  /// Represents an XML value being constructed during serialization.
  /// Wraps a reference to the pugixml node representing the value.
  struct XMLOutputVar {
    XMLOutputVar(const Ref<pugi::xml_node>& _node) : node_(_node) {}
    Ref<pugi::xml_node> node_;
  };

  using OutputArrayType = XMLOutputArray;
  using OutputObjectType = XMLOutputObject;
  using OutputVarType = XMLOutputVar;

  /// Constructs an XML Writer with the specified root node and name.
  /// @param _root Reference to the pugixml node that will serve as the document root
  /// @param _root_name The name for the root element
  Writer(const Ref<pugi::xml_node>& _root, const std::string& _root_name);

  ~Writer();

  /// Creates an XML array as the root element of the output.
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @return An output array that can be populated with elements
  OutputArrayType array_as_root(const size_t _size) const;

  /// Creates an XML object as the root element of the output.
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @return An output object that can be populated with child elements and attributes
  OutputObjectType object_as_root(const size_t _size) const;

  /// Creates a null value as the root element of the output.
  /// @return An output variable representing the null value
  OutputVarType null_as_root() const;

  /// Creates a value as the root element of the output.
  /// Supports basic types like strings, numbers, and booleans.
  /// @tparam T The type of the value to serialize
  /// @param _var The value to write as the root element
  /// @return An output variable representing the serialized value
  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    const auto str = to_string(_var);
    return value_as_root_impl(str);
  }

  /// Adds a nested array to a parent array.
  /// Creates a new sibling element with the same name as the parent array.
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @param _parent Pointer to the parent array to add to
  /// @return An output array that can be populated with elements
  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  /// Adds a nested array to a parent object with the specified field name.
  /// @param _name The name of the field in the parent object (used as element name)
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @param _parent Pointer to the parent object to add to
  /// @return An output array that can be populated with elements
  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  /// Adds an XML comment to a parent array.
  /// @param _comment The comment text to add
  /// @param _parent Pointer to the parent array to add the comment to
  void add_comment_to_array(const std::string_view& _comment,
                            OutputArrayType* _parent) const;

  /// Adds an XML comment to a parent object.
  /// @param _comment The comment text to add
  /// @param _parent Pointer to the parent object to add the comment to
  void add_comment_to_object(const std::string_view& _comment,
                             OutputObjectType* _parent) const;

  /// Adds a nested object to a parent array.
  /// Creates a new sibling element with the same name as the parent array.
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @param _parent Pointer to the parent array to add to
  /// @return An output object that can be populated with child elements and attributes
  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  /// Adds a nested object to a parent object with the specified field name.
  /// @param _name The name of the field in the parent object (used as element name)
  /// @param _size The expected size (unused, reserved for future optimization)
  /// @param _parent Pointer to the parent object to add to
  /// @return An output object that can be populated with child elements and attributes
  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  /// Adds a value to a parent array.
  /// Creates a new sibling element with the same name as the parent array.
  /// Supports basic types like strings, numbers, and booleans.
  /// @tparam T The type of the value to add
  /// @param _var The value to add to the array
  /// @param _parent Pointer to the parent array to add to
  /// @return An output variable representing the added value
  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    const auto str = to_string(_var);
    return add_value_to_array_impl(str, _parent);
  }

  /// Adds a value to a parent object with the specified field name.
  /// Can be added as either a child element or an attribute.
  /// Supports basic types like strings, numbers, and booleans.
  /// @tparam T The type of the value to add
  /// @param _name The name of the field in the parent object
  /// @param _var The value to add to the object
  /// @param _parent Pointer to the parent object to add to
  /// @param _is_attribute If true, adds as an XML attribute; if false, adds as a child element
  /// @return An output variable representing the added value
  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var, OutputObjectType* _parent,
                                    const bool _is_attribute = false) const {
    const auto str = to_string(_var);
    return add_value_to_object_impl(_name, str, _parent, _is_attribute);
  }

  /// Adds a null value to a parent array.
  /// @param _parent Pointer to the parent array to add to
  /// @return An output variable representing the null value
  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  /// Adds a null value to a parent object with the specified field name.
  /// Can be added as either a child element or an attribute.
  /// @param _name The name of the field in the parent object
  /// @param _parent Pointer to the parent object to add to
  /// @param _is_attribute If true, adds as an XML attribute; if false, adds as a child element
  /// @return An output variable representing the null value
  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent,
                                   const bool _is_attribute = false) const;

  /// Finalizes an XML array after all elements have been added.
  /// This is a no-op for XML as arrays don't need explicit finalization.
  /// @param _arr Pointer to the array to finalize
  void end_array(OutputArrayType* _arr) const;

  /// Finalizes an XML object after all fields have been added.
  /// This is a no-op for XML as objects don't need explicit finalization.
  /// @param _obj Pointer to the object to finalize
  void end_object(OutputObjectType* _obj) const;

 private:
  template <class T>
  decltype(auto) to_string(const T& _val) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return _val;  // Return reference to avoid expensive string copy.
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return _val ? "true" : "false";
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      return std::to_string(_val);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  }

  OutputVarType value_as_root_impl(const std::string& _str) const;

  OutputVarType add_value_to_array_impl(const std::string& _str,
                                        OutputArrayType* _parent) const;

  OutputVarType add_value_to_object_impl(
      const std::string_view& _name, const std::string& _str,
      OutputObjectType* _parent, const bool _is_attribute = false) const;

 public:
  Ref<pugi::xml_node> root_;

  std::string root_name_;
};

}  // namespace rfl::xml

#endif  // RFL_XML_WRITER_HPP_
