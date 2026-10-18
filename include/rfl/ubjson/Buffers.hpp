#ifndef RFL_UBJSON_BUFFERS_HPP_
#define RFL_UBJSON_BUFFERS_HPP_

#include <istream>
#include <memory>
#include <vector>

#include "../Result.hpp"
#include "../common.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::ubjson::detail {
RFL_API Result<std::shared_ptr<jsoncons::json>> decode(const char* _bytes,
                                                       size_t _size);
RFL_API Result<std::shared_ptr<jsoncons::json>> decode(std::istream& _stream);

class RFL_API EncodedBuffer {
 public:
  EncodedBuffer();
  ~EncodedBuffer();
  EncodedBuffer(const EncodedBuffer&) = delete;
  EncodedBuffer& operator=(const EncodedBuffer&) = delete;
  jsoncons::ubjson::ubjson_bytes_encoder* encoder() const;
  std::vector<char> bytes() const;

 private:
  struct Impl;
  Impl* impl_;
};
}  // namespace rfl::ubjson::detail

#endif
