#ifndef RFL_BSON_WRITE_HPP_
#define RFL_BSON_WRITE_HPP_

#include <bson/bson.h>

#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../internal/no_field_names_v.hpp"
#include "../internal/ptr_cast.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace bson {

/// Converts an object to BSON and returns a buffer pointer.
/// WARNING: It is the caller's responsibility to call bson_free() on the returned pointer.
/// This is a low-level function; prefer write() for automatic memory management.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to BSON
/// @return Result containing a pair of (buffer pointer, buffer size) or an error
template <class... Ps>
Result<std::pair<uint8_t*, size_t>> to_buffer(const auto& _obj) noexcept {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  bson_t* doc = nullptr;
  uint8_t* buf = nullptr;
  size_t buflen = 0;
  auto bson_writer = std::shared_ptr<bson_writer_t>(
      bson_writer_new(&buf, &buflen, 0, bson_realloc_ctx, NULL),
      bson_writer_destroy);
  bson_writer_begin(bson_writer.get(), &doc);
  const auto rfl_writer = Writer(doc);
  using ProcessorsType = Processors<Ps...>;
  static_assert(!internal::no_field_names_v<ProcessorsType>,
                "The NoFieldNames processor is not supported for BSON, XML, "
                "TOML, or YAML.");
  const auto nothing = [&]() -> Result<Nothing> {
    try {
      Parser<T, ProcessorsType>::write(rfl_writer, _obj,
                                       typename ParentType::Root{});
      return Nothing{};
    } catch (const std::exception& e) {
      return error(e.what());
    }
  }();
  bson_writer_end(bson_writer.get());
  const auto len = bson_writer_get_length(bson_writer.get());
  return nothing
      .transform([&](const auto&) { return std::make_pair(buf, len); })
      .or_else([&](auto&& _err) {
        bson_free(buf);
        return error(_err.what());
      });
}

/// Returns BSON bytes representation of the object.
/// BSON (Binary JSON) is the binary serialization format used by MongoDB.
/// Uses compile-time reflection to serialize a C++ object to BSON format.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to BSON
/// @return A vector of chars containing the BSON binary representation
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  return to_buffer<Ps...>(_obj)
      .transform([](const auto& _p) {
        const auto vec =
            std::vector<char>(internal::ptr_cast<char*>(_p.first),
                              internal::ptr_cast<char*>(_p.first) + _p.second);
        bson_free(_p.first);
        return vec;
      })
      .value();
}

/// Writes a BSON representation into an ostream.
/// Uses compile-time reflection to serialize a C++ object to BSON and write to a stream.
/// @tparam Ps Processors to apply during serialization (transforms the data)
/// @param _obj The object to serialize to BSON
/// @param _stream The output stream to write BSON binary data to
/// @return The output stream (for chaining)
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  to_buffer<Ps...>(_obj)
      .transform([&](const auto& _p) {
        _stream.write(internal::ptr_cast<const char*>(_p.first), _p.second);
        bson_free(_p.first);
        return Nothing{};
      })
      .value();
  return _stream;
}

}  // namespace bson
}  // namespace rfl

#endif
