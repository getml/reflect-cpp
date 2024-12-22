#include "rfl/avro/SchemaImpl.hpp"

namespace rfl::avro {

SchemaImpl::SchemaImpl(const std::string& _json_str)
    : json_str_(_json_str),
      schema_(Box<avro_schema_t>::make()),
      iface_(nullptr) {
  const auto err = avro_schema_from_json_length(
      _json_str.c_str(), _json_str.size(), schema_.get());
  if (err) {
    throw std::runtime_error(
        "Cannot create schema from the following JSON: \"" + _json_str +
        "\": " + avro_strerror());
  }
  iface_ = avro_generic_class_from_schema(*schema_);
}

SchemaImpl::~SchemaImpl() {
  if (iface_) {
    avro_value_iface_decref(iface_);
    iface_ = nullptr;
    avro_schema_decref(*schema_);
  }
}

SchemaImpl::SchemaImpl(SchemaImpl&& _other) noexcept
    : json_str_(std::move(_other.json_str_)),
      schema_(std::move(_other.schema_)),
      iface_(_other.iface_) {
  _other.iface_ = nullptr;
}

SchemaImpl& SchemaImpl::operator=(SchemaImpl&& _other) noexcept {
  if (this == &_other) {
    return *this;
  }
  json_str_ = std::move(_other.json_str_);
  schema_ = std::move(_other.schema_);
  iface_ = _other.iface_;
  return *this;
}

}  // namespace rfl::avro
