#ifndef RFL_YAML_WRITER_HPP_
#define RFL_YAML_WRITER_HPP_

#include <yaml-cpp/yaml.h>

#include <exception>
#include <map>
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
namespace yaml {

class Writer {
 public:
  struct YAMLArray {};

  struct YAMLObject {};

  struct YAMLVar {};

  using OutputArrayType = YAMLArray;
  using OutputObjectType = YAMLObject;
  using OutputVarType = YAMLVar;

  Writer(const Ref<YAML::Emitter>& _out) : out_(_out) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    return new_array();
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return new_object();
  }

  OutputVarType null_as_root() const noexcept {
    return insert_value(YAML::Null);
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    return insert_value(_var);
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    return new_array();
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return new_array(_name);
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    return new_object();
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return new_object(_name);
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    return insert_value(_var);
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    return insert_value(_name, _var);
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    return insert_value(YAML::Null);
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    return insert_value(_name, YAML::Null);
  }

  void end_array(OutputArrayType* _arr) const noexcept {
    (*out_) << YAML::EndSeq;
  }

  void end_object(OutputObjectType* _obj) const noexcept {
    (*out_) << YAML::EndMap;
  }

 private:
  template <class T>
  OutputVarType insert_value(const std::string_view& _name,
                             const T& _var) const noexcept {
    (*out_) << YAML::Key << _name.data() << YAML::Value << _var;
    return OutputVarType{};
  }

  template <class T>
  OutputVarType insert_value(const T& _var) const noexcept {
    (*out_) << _var;
    return OutputVarType{};
  }

  OutputArrayType new_array(const std::string_view& _name) const noexcept {
    (*out_) << YAML::Key << _name.data() << YAML::Value << YAML::BeginSeq;
    return OutputArrayType{};
  }

  OutputArrayType new_array() const noexcept {
    (*out_) << YAML::BeginSeq;
    return OutputArrayType{};
  }

  OutputObjectType new_object(const std::string_view& _name) const noexcept {
    (*out_) << YAML::Key << _name.data() << YAML::Value << YAML::BeginMap;
    return OutputObjectType{};
  }

  OutputObjectType new_object() const noexcept {
    (*out_) << YAML::BeginMap;
    return OutputObjectType{};
  }

 public:
  const Ref<YAML::Emitter> out_;
};

}  // namespace yaml
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
