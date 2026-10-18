#ifndef RFL_MSGPACK_BUFFERS_HPP_
#define RFL_MSGPACK_BUFFERS_HPP_

#include <msgpack.h>

#include <cstddef>
#include <vector>

#include "../common.hpp"

namespace rfl::msgpack::detail {

// Keep MessagePack's static inline buffer functions in an implementation TU.
class RFL_API PackedBuffer {
 public:
  PackedBuffer();
  ~PackedBuffer();
  PackedBuffer(const PackedBuffer&) = delete;
  PackedBuffer& operator=(const PackedBuffer&) = delete;

  msgpack_packer* packer();
  std::vector<char> bytes() const;

 private:
  msgpack_sbuffer buffer_;
  msgpack_packer packer_;
};

class RFL_API UnpackedBuffer {
 public:
  UnpackedBuffer(const char* _bytes, std::size_t _size);
  ~UnpackedBuffer();
  UnpackedBuffer(const UnpackedBuffer&) = delete;
  UnpackedBuffer& operator=(const UnpackedBuffer&) = delete;

  bool valid() const;
  const msgpack_object& object() const;

 private:
  msgpack_zone zone_;
  msgpack_object object_;
  bool valid_;
};

}  // namespace rfl::msgpack::detail

#endif
