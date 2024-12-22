#include "rfl/capnproto/SchemaImpl.hpp"

#include <capnp/schema-parser.h>
#include <capnp/schema.h>

namespace rfl::capnproto {

SchemaImpl::SchemaImpl(const std::string& _str)
    : str_(_str), schema_(Box<capnp::ParsedSchema>::make(make_schema(_str))) {}

capnp::ParsedSchema SchemaImpl::make_schema(const std::string& _str) {
  auto dir = kj::newInMemoryDirectory(kj::nullClock());
  auto path = kj::Path::parse("foo/bar.capnp");
  dir->openFile(path, kj::WriteMode::CREATE | kj::WriteMode::CREATE_PARENT)
      ->writeAll(_str);
  capnp::SchemaParser parser;
  return parser.parseFromDirectory(*dir, std::move(path), nullptr);
}

}  // namespace rfl::capnproto
