#include "rfl/yaml/Writer.hpp"

namespace rfl::yaml {

Writer::Writer(const Ref<YAML::Emitter>& _out) : out_(_out) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(const size_t /*_size*/) const {
  return new_array();
}

Writer::OutputObjectType Writer::object_as_root(const size_t /*_size*/) const {
  return new_object();
}

Writer::OutputVarType Writer::null_as_root() const {
  return insert_value(YAML::Null);
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t /*_size*/, OutputArrayType* /*_parent*/) const {
  return new_array();
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* /*_parent*/) const {
  return new_array(_name);
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t /*_size*/, OutputArrayType* /*_parent*/) const {
  return new_object();
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* /*_parent*/) const {
  return new_object(_name);
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* /*_parent*/) const {
  return insert_value(YAML::Null);
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* /*_parent*/) const {
  return insert_value(_name, YAML::Null);
}

void Writer::end_array(OutputArrayType* /*_arr*/) const {
  (*out_) << YAML::EndSeq;
}

void Writer::end_object(OutputObjectType* /*_obj*/) const {
  (*out_) << YAML::EndMap;
}

Writer::OutputArrayType Writer::new_array(const std::string_view& _name) const {
  (*out_) << YAML::Key << make_yaml_string(_name) << YAML::Value << YAML::BeginSeq;
  return OutputArrayType{};
}

Writer::OutputArrayType Writer::new_array() const {
  (*out_) << YAML::BeginSeq;
  return OutputArrayType{};
}

Writer::OutputObjectType Writer::new_object(
    const std::string_view& _name) const {
  (*out_) << YAML::Key << make_yaml_string(_name) << YAML::Value << YAML::BeginMap;
  return OutputObjectType{};
}

Writer::OutputObjectType Writer::new_object() const {
  (*out_) << YAML::BeginMap;
  return OutputObjectType{};
}

}  // namespace rfl::yaml
