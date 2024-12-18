#ifndef RFL_BSON_WRITE_HPP_
#define RFL_BSON_WRITE_HPP_

#include <bson/bson.h>

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "../Processors.hpp"
#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace bson {

/// Returns BSON bytes. Careful: It is the responsibility of the caller to call
/// bson_free on the returned pointer.
template <class... Ps>
std::pair<uint8_t*, size_t> to_buffer(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  bson_t* doc = nullptr;
  uint8_t* buf = nullptr;
  size_t buflen = 0;
  bson_writer_t* bson_writer =
      bson_writer_new(&buf, &buflen, 0, bson_realloc_ctx, NULL);
  bson_writer_begin(bson_writer, &doc);
  const auto rfl_writer = Writer(doc);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!ProcessorsType::no_field_names_,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  Parser<T, ProcessorsType>::write(rfl_writer, _obj,
                                   typename ParentType::Root{});
  bson_writer_end(bson_writer);
  const auto len = bson_writer_get_length(bson_writer);
  bson_writer_destroy(bson_writer);
  return std::make_pair(buf, len);
}

/// Returns BSON bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) noexcept {
  auto [buf, len] = to_buffer<Ps...>(_obj);
  const auto result = std::vector<char>(internal::ptr_cast<char*>(buf),
                                        internal::ptr_cast<char*>(buf) + len);
  bson_free(buf);
  return result;
}

/// Writes a BSON into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) noexcept {
  auto [buf, len] = to_buffer<Ps...>(_obj);
  _stream.write(internal::ptr_cast<const char*>(buf), len);
  bson_free(buf);
  return _stream;
}

}  // namespace bson
}  // namespace rfl

#endif
