#ifndef GENERIC_WRITER_HPP_
#define GENERIC_WRITER_HPP_

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Generic.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl::generic {

/// Writer for the Generic type - a dynamic, format-agnostic data structure.
/// Constructs Generic values that can be converted to any serialization format.
/// The Generic type provides a common intermediate representation useful for
/// format conversions and dynamic data manipulation.
struct RFL_API Writer {
  struct OutputArray {
    Generic::Array* val_;
  };

  struct OutputObject {
    Generic::Object* val_;
  };

  using OutputArrayType = OutputArray;
  using OutputObjectType = OutputObject;
  using OutputVarType = Generic;

  Writer() {}

  ~Writer() = default;

  /// Creates an array as the root element of the Generic structure.
  /// @param _size The number of elements (hint, not enforced)
  /// @return An OutputArrayType that can be populated with elements
  OutputArrayType array_as_root(const size_t _size) const noexcept;

  /// Creates an object as the root element of the Generic structure.
  /// @param _size The number of fields (hint, not enforced)
  /// @return An OutputObjectType that can be populated with fields
  OutputObjectType object_as_root(const size_t _size) const noexcept;

  /// Creates a null value as the root element.
  /// @return A Generic value representing null
  OutputVarType null_as_root() const noexcept;

  /// Creates a primitive value as the root element.
  /// @tparam T The type of the value (string, bool, number)
  /// @param _var The value to store
  /// @return A Generic value wrapping the primitive
  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    root_ = to_generic(_var);
    return root_;
  }

  /// Adds a nested array to an existing array.
  /// @param _size The number of elements (hint, not enforced)
  /// @param _parent Pointer to the parent array
  /// @return An OutputArrayType for the new nested array
  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept;

  /// Adds an array to an object field.
  /// @param _name The field name for the array
  /// @param _size The number of elements (hint, not enforced)
  /// @param _parent Pointer to the parent object
  /// @return An OutputArrayType for the new array
  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept;

  /// Adds a nested object to an existing array.
  /// @param _size The number of fields (hint, not enforced)
  /// @param _parent Pointer to the parent array
  /// @return An OutputObjectType for the new nested object
  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  /// Adds an object to an object field (nested object).
  /// @param _name The field name for the nested object
  /// @param _size The number of fields (hint, not enforced)
  /// @param _parent Pointer to the parent object
  /// @return An OutputObjectType for the new nested object
  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    const auto g = to_generic(_var);
    _parent->val_->push_back(g);
    return g;
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    const auto g = to_generic(_var);
    _parent->val_->insert(_name, g);
    return g;
  }

  /// Adds a null value to an array.
  /// @param _parent Pointer to the parent array
  /// @return A Generic value representing null
  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  /// Adds a null value to an object field.
  /// @param _name The field name for the null value
  /// @param _parent Pointer to the parent object
  /// @return A Generic value representing null
  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  /// Finalizes an array after all elements have been added.
  /// This is a no-op for Generic but required by the Writer interface.
  /// @param _arr Pointer to the array (unused)
  void end_array(OutputArrayType*) const noexcept {}

  /// Finalizes an object after all fields have been added.
  /// This is a no-op for Generic but required by the Writer interface.
  /// @param _obj Pointer to the object (unused)
  void end_object(OutputObjectType*) const noexcept {}

  /// Gets a reference to the root Generic value being constructed.
  /// @return Reference to the root value
  OutputVarType& root() { return root_; }

 private:
  template <class T>
  OutputVarType to_generic(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return OutputVarType(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return OutputVarType(_var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return OutputVarType(static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return OutputVarType(static_cast<int64_t>(_var));
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  }

 private:
  mutable OutputVarType root_;
};

}  // namespace rfl::generic

#endif
