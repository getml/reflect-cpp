#include "rfl/msgpack/Buffers.hpp"

namespace rfl::msgpack::detail {

PackedBuffer::PackedBuffer() {
  msgpack_sbuffer_init(&buffer_);
  msgpack_packer_init(&packer_, &buffer_, msgpack_sbuffer_write);
}

PackedBuffer::~PackedBuffer() { msgpack_sbuffer_destroy(&buffer_); }

msgpack_packer* PackedBuffer::packer() { return &packer_; }

std::vector<char> PackedBuffer::bytes() const {
  return std::vector<char>(buffer_.data, buffer_.data + buffer_.size);
}

UnpackedBuffer::UnpackedBuffer(const char* _bytes, std::size_t _size) {
  msgpack_zone_init(&zone_, 2048);
  const auto result = msgpack_unpack(_bytes, _size, nullptr, &zone_, &object_);
  valid_ =
      result == MSGPACK_UNPACK_SUCCESS || result == MSGPACK_UNPACK_EXTRA_BYTES;
}

UnpackedBuffer::~UnpackedBuffer() { msgpack_zone_destroy(&zone_); }

bool UnpackedBuffer::valid() const { return valid_; }

const msgpack_object& UnpackedBuffer::object() const { return object_; }

}  // namespace rfl::msgpack::detail
