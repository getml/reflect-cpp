#include "rfl/cbor/Buffers.hpp"

#include <jsoncons_ext/cbor/decode_cbor.hpp>

namespace rfl::cbor::detail {
namespace {
template <class Source>
Result<std::shared_ptr<jsoncons::json>> decode_source(const Source& _source) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_source);
  if (!result.has_value())
    return error("Could not parse CBOR: " + result.error().message());
  return std::make_shared<jsoncons::json>(std::move(result.value()));
}
}  // namespace
Result<std::shared_ptr<jsoncons::json>> decode(const char* _bytes,
                                               size_t _size) {
  return decode_source(jsoncons::byte_string_view(
      reinterpret_cast<const uint8_t*>(_bytes), _size));
}
Result<std::shared_ptr<jsoncons::json>> decode(std::istream& _stream) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_stream);
  if (!result.has_value())
    return error("Could not parse CBOR: " + result.error().message());
  return std::make_shared<jsoncons::json>(std::move(result.value()));
}
struct EncodedBuffer::Impl {
  std::vector<uint8_t> buffer;
  jsoncons::cbor::cbor_bytes_encoder encoder{buffer};
};
EncodedBuffer::EncodedBuffer() : impl_(new Impl) {}
EncodedBuffer::~EncodedBuffer() { delete impl_; }
jsoncons::cbor::cbor_bytes_encoder* EncodedBuffer::encoder() const {
  return &impl_->encoder;
}
std::vector<char> EncodedBuffer::bytes() const {
  return {impl_->buffer.begin(), impl_->buffer.end()};
}
}  // namespace rfl::cbor::detail
