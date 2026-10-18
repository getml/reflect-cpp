#include "native_reference.hpp"

#include <capnp/dynamic.h>
#include <capnp/schema-parser.h>
#include <capnp/serialize-packed.h>
#include <kj/io.h>

namespace test_capnproto_bridge {
std::vector<char> reference_bytes(const capnp::ParsedSchema& _schema,
                                  const Record& _record) {
  capnp::MallocMessageBuilder message;
  auto root = message.initRoot<capnp::DynamicStruct>(
      _schema.getAllNested()[0].asStruct());
  root.set("text", _record.text.c_str());
  root.set("count", _record.count);
  root.set("ready", _record.ready);
  auto values =
      root.init("values", _record.values.size()).as<capnp::DynamicList>();
  for (unsigned i = 0; i < values.size(); ++i) values.set(i, _record.values[i]);
  kj::VectorOutputStream stream;
  capnp::writePackedMessage(stream, message);
  const auto bytes = stream.getArray();
  const auto begin = reinterpret_cast<const char*>(bytes.begin());
  return {begin, begin + bytes.size()};
}
Record reference_read(const capnp::ParsedSchema& _schema,
                      const std::vector<char>& _bytes) {
  kj::ArrayInputStream stream(kj::ArrayPtr<const kj::byte>(
      reinterpret_cast<const kj::byte*>(_bytes.data()), _bytes.size()));
  capnp::PackedMessageReader message(stream);
  auto root = message.getRoot<capnp::DynamicStruct>(
      _schema.getAllNested()[0].asStruct());
  Record result{root.get("text").as<capnp::Text>().cStr(),
                root.get("count").as<int64_t>(),
                root.get("ready").as<bool>(),
                {}};
  for (auto value : root.get("values").as<capnp::DynamicList>())
    result.values.push_back(value.as<double>());
  return result;
}
}  // namespace test_capnproto_bridge
