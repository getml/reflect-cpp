#ifndef RFL_YAML_WRITER_HPP_
#define RFL_YAML_WRITER_HPP_

#include <yaml-cpp/yaml.h>

#include <string>
#include <string_view>
#include <type_traits>

#include "../Ref.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl::yaml {

class RFL_API Writer {
 public:
  /**
   * @enum Flags
   * @brief Flags to control YAML string formatting behavior in Writer.
   */
  enum Flags {
    no_flags = 0,  ///< No special formatting flags.

    /// @brief Write string values containing new-lines as YAML multiline
    /// literals.
    /// @details Costs one call to std::basic_string::find on all string values.
    string_multiline_literal = 1,

    /// @brief Write all string values as YAML multiline literals.
    string_all_literal = 2
  };

  /**
   * @struct YAMLArray
   * @brief Tag type representing a YAML array node.
   */
  struct YAMLArray {};

  /**
   * @struct YAMLObject
   * @brief Tag type representing a YAML object node.
   */
  struct YAMLObject {};

  /**
   * @struct YAMLVar
   * @brief Tag type representing a YAML value node.
   */
  struct YAMLVar {};

  using OutputArrayType = YAMLArray;    ///< Alias for YAML array output type.
  using OutputObjectType = YAMLObject;  ///< Alias for YAML object output type.
  using OutputVarType = YAMLVar;        ///< Alias for YAML value output type.

  /// @brief Constructs a Writer for emitting YAML.
  /// @param _out Reference to a YAML::Emitter object for output.
  /// @param _flags Optional flags to control string formatting.
  Writer(const Ref<YAML::Emitter>& _out, Flags _flags = no_flags);

  /// @brief Destructor for Writer.
  ~Writer();

  /// @brief Begin a YAML array as the root node.
  /// @param _size Expected size of the array.
  /// @return OutputArrayType representing the root array.
  OutputArrayType array_as_root(const size_t _size) const;

  /// @brief Begin a YAML object as the root node.
  /// @param _size Expected size of the object.
  /// @return OutputObjectType representing the root object.
  OutputObjectType object_as_root(const size_t _size) const;

  /// @brief Begin a YAML null value as the root node.
  /// @return OutputVarType representing the root null value.
  OutputVarType null_as_root() const;

  /// @brief Begin a YAML value as the root node.
  /// @tparam T Type of the value.
  /// @param _var Value to write as root.
  /// @return OutputVarType representing the root value.
  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    return insert_value(_var);
  }

  /// @brief Add a new array to an existing array node.
  /// @param _size Expected size of the new array.
  /// @param _parent Pointer to the parent array node.
  /// @return OutputArrayType representing the new array.
  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* /*_parent*/) const;

  /// @brief Add a new array to an object node under a given key.
  /// @param _name Name of the key in the object.
  /// @param _size Expected size of the new array.
  /// @param _parent Pointer to the parent object node.
  /// @return OutputArrayType representing the new array.
  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* /*_parent*/) const;

  /// @brief Add a comment to an array node.
  /// @param _comment The comment string.
  /// @param _parent Pointer to the parent array node.
  void add_comment_to_array(const std::string_view& _comment,
                            OutputArrayType* _parent) const;

  /// @brief Add a comment to an object node.
  /// @param _comment The comment string.
  /// @param _parent Pointer to the parent object node.
  void add_comment_to_object(const std::string_view& _comment,
                             OutputObjectType* _parent) const;

  /// @brief Add a new object to an array node.
  /// @param _size Expected size of the new object.
  /// @param _parent Pointer to the parent array node.
  /// @return OutputObjectType representing the new object.
  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  /// @brief Add a new object to an object node under a given key.
  /// @param _name Name of the key in the object.
  /// @param _size Expected size of the new object.
  /// @param _parent Pointer to the parent object node.
  /// @return OutputObjectType representing the new object.
  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  /// @brief Add a value to an array node.
  /// @tparam T Type of the value.
  /// @param _var Value to add.
  /// @param _parent Pointer to the parent array node.
  /// @return OutputVarType representing the added value.
  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    return insert_value(_var);
  }

  /// @brief Add a value to an object node under a given key.
  /// @tparam T Type of the value.
  /// @param _name Name of the key in the object.
  /// @param _var Value to add.
  /// @param _parent Pointer to the parent object node.
  /// @return OutputVarType representing the added value.
  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    return insert_value(_name, _var);
  }

  /// @brief Add a YAML null value to an array node.
  /// @param _parent Pointer to the parent array node.
  /// @return OutputVarType representing the added null value.
  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  /// @brief Add a YAML null value to an object node under a given key.
  /// @param _name Name of the key in the object.
  /// @param _parent Pointer to the parent object node.
  /// @return OutputVarType representing the added null value.
  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  /// @brief End the current array node.
  /// @param _arr Pointer to the array node to end.
  void end_array(OutputArrayType* _arr) const;

  /// @brief End the current object node.
  /// @param _obj Pointer to the object node to end.
  void end_object(OutputObjectType* _obj) const;

 private:
  /// @brief Insert YAML literal block indicator if needed for a value.
  /// @tparam T Type of the value.
  /// @param _var Value to check for literal block formatting.
  template <class T>
  void insert_literal_block_if_needed(const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      if (flags_ & string_all_literal ||
          (flags_ & string_multiline_literal &&
           _var.find('\n') != std::string::npos)) {
        (*out_) << YAML::Literal;
      }
    }
  }

 public:
  /// @brief Insert a value into an object node under a given key.
  /// @tparam T Type of the value.
  /// @param _name Name of the key in the object.
  /// @param _var Value to insert.
  /// @return OutputVarType representing the inserted value.
  template <class T>
  OutputVarType insert_value(const std::string_view& _name,
                             const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>() ||
                  std::is_same<std::remove_cvref_t<T>, bool>() ||
                  std::is_same<std::remove_cvref_t<T>,
                               std::remove_cvref_t<decltype(YAML::Null)>>()) {
      (*out_) << YAML::Key << std::string(_name) << YAML::Value;
      insert_literal_block_if_needed(_var);
      (*out_) << _var;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      // std::to_string is necessary to ensure that floating point values are
      // always written as floats.
      (*out_) << YAML::Key << std::string(_name) << YAML::Value
              << std::to_string(_var);
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      (*out_) << YAML::Key << std::string(_name) << YAML::Value
              << static_cast<int64_t>(_var);
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  /// @brief Insert a value node.
  /// @tparam T Type of the value.
  /// @param _var Value to insert.
  /// @return OutputVarType representing the inserted value.
  template <class T>
  OutputVarType insert_value(const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>() ||
                  std::is_same<std::remove_cvref_t<T>, bool>() ||
                  std::is_same<std::remove_cvref_t<T>,
                               std::remove_cvref_t<decltype(YAML::Null)>>()) {
      insert_literal_block_if_needed(_var);
      (*out_) << _var;
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      // std::to_string is necessary to ensure that floating point values are
      // always written as floats.
      (*out_) << std::to_string(_var);
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      (*out_) << static_cast<int64_t>(_var);
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  /// @brief Create a new YAML array node under a given key.
  /// @param _name Name of the key in the object.
  /// @return OutputArrayType representing the new array.
  OutputArrayType new_array(const std::string_view& _name) const;

  /// @brief Create a new YAML array node as a root or child.
  /// @return OutputArrayType representing the new array.
  OutputArrayType new_array() const;

  /// @brief Add a comment to the YAML output.
  /// @param _comment The comment string.
  void new_comment(const std::string_view& _comment) const;

  /// @brief Create a new YAML object node under a given key.
  /// @param _name Name of the key in the object.
  /// @return OutputObjectType representing the new object.
  OutputObjectType new_object(const std::string_view& _name) const;

  /// @brief Create a new YAML object node as a root or child.
  /// @return OutputObjectType representing the new object.
  OutputObjectType new_object() const;

 public:
  /// @brief Reference to the YAML emitter used for output.
  const Ref<YAML::Emitter> out_;
  /// @brief Flags controlling string formatting behavior.
  Flags flags_;
};

}  // namespace rfl::yaml

#endif
