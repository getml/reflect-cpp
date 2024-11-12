#ifndef RFL_AVRO_SCHEMA_HPP_
#define RFL_AVRO_SCHEMA_HPP_

#include <avro.h>

#include <string>

#include "../Box.hpp"
#include "../Result.hpp"

namespace rfl::avro {

class Schema {
 public:
  Schema(const std::string& _json_str);

  static Result<Schema> from_json(const std::string& _json_str) noexcept;

  ~Schema();

  Schema(const Schema& _other) = delete;

  Schema(Schema&& _other) noexcept;

  Schema& operator=(const Schema& _other) = delete;

  Schema& operator=(Schema&& _other) noexcept;

  /// The JSON string used to create this schema.
  const std::string& json_str() const { return json_str_; }

  /// The interface used to create new values.
  const avro_value_iface_t* iface() const { return iface_; };

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
