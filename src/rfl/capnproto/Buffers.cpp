#include "rfl/capnproto/Buffers.hpp"

#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <kj/io.h>

namespace rfl::capnproto::detail {
struct PackedReader::Impl {
  Impl(const void* _bytes, size_t _size, const capnp::ParsedSchema& _schema,
       const std::string& _name)
      : stream(kj::ArrayPtr<const kj::byte>(
            static_cast<const kj::byte*>(_bytes), _size)),
        message(stream),
        root(message.getRoot<capnp::DynamicStruct>(
            _schema.getNested(_name.c_str()).asStruct())) {}
  kj::ArrayInputStream stream;
  capnp::PackedMessageReader message;
  capnp::DynamicStruct::Reader root;
};
PackedReader::PackedReader(const void* _bytes, size_t _size,
                           const capnp::ParsedSchema& _schema,
                           const std::string& _name)
    : impl_(std::make_unique<Impl>(_bytes, _size, _schema, _name)) {}
PackedReader::~PackedReader() = default;
Reader::InputVarType PackedReader::root() const { return {impl_->root}; }

struct PackedWriter::Impl {
  Impl(const capnp::ParsedSchema& _schema, const std::string& _name)
      : root(message.initRoot<capnp::DynamicStruct>(
            _schema.getNested(_name.c_str()).asStruct())) {}
  capnp::MallocMessageBuilder message;
  capnp::DynamicStruct::Builder root;
};
PackedWriter::PackedWriter(const capnp::ParsedSchema& _schema,
                           const std::string& _name)
    : impl_(std::make_unique<Impl>(_schema, _name)) {}
PackedWriter::~PackedWriter() = default;
Writer PackedWriter::writer() const { return Writer(&impl_->root); }
std::vector<char> PackedWriter::bytes() const {
  kj::VectorOutputStream stream;
  capnp::writePackedMessage(stream, impl_->message);
  const auto bytes = stream.getArray();
  const auto begin = internal::ptr_cast<const char*>(bytes.begin());
  return {begin, begin + bytes.size()};
}
}  // namespace rfl::capnproto::detail
