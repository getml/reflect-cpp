#ifndef RFL_YAML_WRITER_HPP_
#define RFL_YAML_WRITER_HPP_

#include <yaml-cpp/yaml.h>

#include <string>
#include <string_view>
#include <type_traits>

#include "../Ref.hpp"
#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl {
namespace yaml {

class RFL_API Writer {
 public:
  struct YAMLArray {};

  struct YAMLObject {};

  struct YAMLVar {};

  using OutputArrayType = YAMLArray;
  using OutputObjectType = YAMLObject;
  using OutputVarType = YAMLVar;

  Writer(const Ref<YAML::Emitter>& _out);

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t _size) const;

  OutputVarType null_as_root() const;

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    return insert_value(_var);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const {
    return insert_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const {
    return insert_value(_name, _var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  void end_array(OutputArrayType* _arr) const;

  void end_object(OutputObjectType* _obj) const;

  template <class T>
  OutputVarType insert_value(const std::string_view& _name,
                             const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>() ||
                  std::is_same<std::remove_cvref_t<T>, bool>() ||
                  std::is_same<std::remove_cvref_t<T>,
                               std::remove_cvref_t<decltype(YAML::Null)>>()) {
      (*out_) << YAML::Key << std::string(_name) << YAML::Value << _var;
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

  template <class T>
  OutputVarType insert_value(const T& _var) const {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>() ||
                  std::is_same<std::remove_cvref_t<T>, bool>() ||
                  std::is_same<std::remove_cvref_t<T>,
                               std::remove_cvref_t<decltype(YAML::Null)>>()) {
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

  OutputArrayType new_array(const std::string_view& _name) const;

  OutputArrayType new_array() const;

  OutputObjectType new_object(const std::string_view& _name) const;

  OutputObjectType new_object() const;

 public:
  const Ref<YAML::Emitter> out_;
};

}  // namespace yaml
}  // namespace rfl

#endif
