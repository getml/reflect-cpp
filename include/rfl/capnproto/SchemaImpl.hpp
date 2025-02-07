#ifndef RFL_CAPNPROTO_SCHEMAIMPL_HPP_
#define RFL_CAPNPROTO_SCHEMAIMPL_HPP_

#include <capnp/schema-parser.h>

#include <memory>
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
  const capnp::ParsedSchema& value() const { return *schema_; };

 private:
  static capnp::ParsedSchema make_schema(const std::string& _str,
                                         capnp::SchemaParser* _parser);

 private:
  /// The string used to create the schema.
  std::string str_;

  /// The schema parser - we need to hold onto this during the lifetime of the
  /// schema.
  Box<capnp::SchemaParser> schema_parser_;

  /// The actual schema
  Box<capnp::ParsedSchema> schema_;
};

}  // namespace rfl::capnproto

#endif
