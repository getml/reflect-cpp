#ifndef RFL_CAPNPROTO_SCHEMAIMPL_HPP_
#define RFL_CAPNPROTO_SCHEMAIMPL_HPP_

#ifndef RFL_CAPNPROTO_MODULE_INTERFACE
#include <capnp/schema-parser.h>
#else
namespace capnp {
class SchemaParser;
class ParsedSchema;
}  // namespace capnp
#endif

#include <string>

#include "../Box.hpp"
#include "../common.hpp"

namespace rfl::capnproto {

class RFL_API SchemaImpl {
 public:
  SchemaImpl(const std::string& _str);

  ~SchemaImpl();

  /// The JSON string used to create this schema.
  const std::string& str() const { return str_; }

  /// The interface used to create new values.
  const capnp::ParsedSchema& value() const;

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
