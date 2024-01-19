#ifndef RFL_BSON_WRITE_HPP_
#define RFL_BSON_WRITE_HPP_

#include <bson/bson.h>

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace bson {

/// Returns BSON bytes. Careful: It is the responsibility of the caller to call
/// bson_free on the returned pointer.
template <class T>
std::pair<uint8_t*, size_t> to_buffer(const T& _obj) noexcept {
  using ParentType = parsing::Parent<Writer>;
  bson_t* doc = nullptr;
  uint8_t* buf = nullptr;
  size_t buflen = 0;
  bson_writer_t* bson_writer =
      bson_writer_new(&buf, &buflen, 0, bson_realloc_ctx, NULL);
  bson_writer_begin(bson_writer, &doc);
  const auto rfl_writer = Writer(doc);
  Parser<T>::write(rfl_writer, _obj, typename ParentType::Root{});
  bson_writer_end(bson_writer);
  const auto len = bson_writer_get_length(bson_writer);
  bson_writer_destroy(bson_writer);
  return std::make_pair(buf, len);
}

/// Returns BSON bytes.
template <class T>
std::vector<char> write(const T& _obj) noexcept {
  auto [buf, len] = to_buffer(_obj);
  const auto result = std::vector<char>(reinterpret_cast<char*>(buf),
                                        reinterpret_cast<char*>(buf) + len);
  bson_free(buf);
  return result;
}

/// Writes a BSON into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) noexcept {
  auto [buf, len] = to_buffer(_obj);
  _stream.write(reinterpret_cast<const char*>(buf), len);
  bson_free(buf);
  return _stream;
}

}  // namespace bson
}  // namespace rfl

#endif  // BSON_PARSER_HPP_
