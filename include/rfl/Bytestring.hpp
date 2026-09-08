#ifndef RFL_BYTESTRING_HPP_
#define RFL_BYTESTRING_HPP_

#include <cstddef>
#include <vector>

namespace rfl {

/// Type alias for a vector of bytes.
/// Bytestring represents binary data as a sequence of std::byte values.
/// This is useful for handling raw binary data, especially in formats that distinguish
/// between text strings and binary data (like MessagePack, CBOR, etc.).
using Bytestring = std::vector<std::byte>;

}  // namespace rfl

#endif
