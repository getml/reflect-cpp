#ifndef RFL_AVRO_SCHEMAIMPL_HPP_
#define RFL_AVRO_SCHEMAIMPL_HPP_

#include <avro.h>

#include <string>

#include "../Box.hpp"
#include "../Result.hpp"

namespace rfl::avro {

class SchemaImpl {
 public:
  SchemaImpl(const std::string& _json_str);

  ~SchemaImpl();

  SchemaImpl(const SchemaImpl& _other) = delete;

  SchemaImpl(SchemaImpl&& _other) noexcept;

  SchemaImpl& operator=(const SchemaImpl& _other) = delete;

  SchemaImpl& operator=(SchemaImpl&& _other) noexcept;

  /// The JSON string used to create this schema.
  const std::string& json_str() const { return json_str_; }

  /// The interface used to create new values.
  avro_value_iface_t* iface() const { return iface_; };

 private:
  /// The JSON string used to create the schema.
  std::string json_str_;

  /// The actual schema
  Box<avro_schema_t> schema_;

  /// The interface used to create new, generic classes.
  avro_value_iface_t* iface_;
};

}  // namespace rfl::avro

#endif
