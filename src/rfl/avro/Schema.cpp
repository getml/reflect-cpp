#include "rfl/avro/Schema.hpp"

namespace rfl::avro {

Schema::Schema(const std::string& _json_str)
    : json_str_(_json_str),
      schema_(Box<avro_schema_t>::make()),
      iface_(nullptr) {
  if (avro_schema_from_json_literal(_json_str.c_str(), schema_.get())) {
    throw std::runtime_error("Cannot create schema from JSON.");
  }
  iface_ = avro_generic_class_from_schema(*schema_);
}

Result<Schema> Schema::from_json(const std::string& _json_str) noexcept {
  try {
    return Schema(_json_str);
  } catch (std::exception& e) {
    return Error(e.what());
  }
}

Schema::~Schema() {
  if (iface_) {
    avro_value_iface_decref(iface_);
    iface_ = nullptr;
    avro_schema_decref(*schema_);
  }
}

Schema::Schema(Schema&& _other) noexcept
    : json_str_(std::move(_other.json_str_)),
      schema_(std::move(_other.schema_)),
      iface_(_other.iface_) {
  _other.iface_ = nullptr;
}

Schema& Schema::operator=(Schema&& _other) noexcept {
  if (this == &_other) {
    return *this;
  }
  json_str_ = std::move(_other.json_str_);
  schema_ = std::move(_other.schema_);
  iface_ = _other.iface_;
  return *this;
}

}  // namespace rfl::avro
