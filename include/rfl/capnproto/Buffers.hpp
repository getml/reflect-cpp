#ifndef RFL_CAPNPROTO_BUFFERS_HPP_
#define RFL_CAPNPROTO_BUFFERS_HPP_

#include <memory>
#include <string>
#include <vector>

#include "../common.hpp"
#include "Reader.hpp"
#include "SchemaImpl.hpp"
#include "Writer.hpp"

namespace rfl::capnproto::detail {
class RFL_API PackedReader {
 public:
  PackedReader(const void*, size_t, const capnp::ParsedSchema&,
               const std::string&);
  ~PackedReader();
  PackedReader(const PackedReader&) = delete;
  PackedReader& operator=(const PackedReader&) = delete;
  Reader::InputVarType root() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
class RFL_API PackedWriter {
 public:
  PackedWriter(const capnp::ParsedSchema&, const std::string&);
  ~PackedWriter();
  PackedWriter(const PackedWriter&) = delete;
  PackedWriter& operator=(const PackedWriter&) = delete;
  Writer writer() const;
  std::vector<char> bytes() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace rfl::capnproto::detail
#endif
