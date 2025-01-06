#include "rfl/capnproto/SchemaImpl.hpp"

#include <capnp/schema-parser.h>
#include <capnp/schema.h>

namespace rfl::capnproto {

SchemaImpl::SchemaImpl(const std::string& _str)
    : str_(_str),
      schema_parser_(Box<capnp::SchemaParser>::make()),
      schema_(Box<capnp::ParsedSchema>::make(
          make_schema(str_, schema_parser_.get()))) {}

capnp::ParsedSchema SchemaImpl::make_schema(
    const std::string& _str, capnp::SchemaParser* _schema_parser) {
  auto dir = kj::newInMemoryDirectory(kj::nullClock());
  auto path = kj::Path::parse("foo/bar.capnp");
  dir->openFile(path, kj::WriteMode::CREATE | kj::WriteMode::CREATE_PARENT)
      ->writeAll(_str);
  return _schema_parser->parseFromDirectory(*dir, std::move(path), nullptr);
}

}  // namespace rfl::capnproto
