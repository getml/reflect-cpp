#ifndef RFL_INTERNAL_JSONCONS_MODULE_FWD_HPP_
#define RFL_INTERNAL_JSONCONS_MODULE_FWD_HPP_

#include <cstdint>
#include <jsoncons/json_fwd.hpp>
#include <memory>
#include <vector>

// Only the canonical pointer types are needed by imported rfl templates.
namespace jsoncons {
using json = basic_json<char>;
template <class Container, class Enable>
class bytes_sink;
namespace cbor {
template <class Sink, class Allocator>
class basic_cbor_encoder;
using cbor_bytes_encoder =
    basic_cbor_encoder<bytes_sink<std::vector<uint8_t>, void>,
                       std::allocator<char>>;
}  // namespace cbor
namespace ubjson {
template <class Sink, class Allocator>
class basic_ubjson_encoder;
using ubjson_bytes_encoder =
    basic_ubjson_encoder<bytes_sink<std::vector<uint8_t>, void>,
                         std::allocator<char>>;
}  // namespace ubjson
}  // namespace jsoncons

#endif
