#ifndef RFL_CAPNPROTO_SCHEMAIMPL_HPP_
#define RFL_CAPNPROTO_SCHEMAIMPL_HPP_

#include <capnp/schema.h>

#include <string>

#include "../Box.hpp"
#include "../Result.hpp"

namespace rfl::capnproto {

class SchemaImpl {
 public:
  SchemaImpl(const std::string& _str);

  ~SchemaImpl() = default;

  /// The JSON string used to create this schema.
  const std::string& str() const { return str_; }

  /// The interface used to create new values.
  const capnp::StructSchema& value() const { return *schema_; };

 private:
  static capnp::StructSchema make_schema(const std::string& _str);

 private:
  /// The string used to create the schema.
  std::string str_;

  /// The actual schema
  Box<capnp::StructSchema> schema_;
};

}  // namespace rfl::capnproto

#endif
