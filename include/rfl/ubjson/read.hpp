#ifndef RFL_UBJSON_READ_HPP_
#define RFL_UBJSON_READ_HPP_

#include <bit>
#include <istream>
#include <string>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Buffers.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::ubjson {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from UBJSON bytes using reflection (contiguous container version).
/// UBJSON (Universal Binary JSON) is a compact binary format that mirrors JSON's data model.
/// It's designed to be faster to parse than JSON while maintaining compatibility with JSON semantics.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _bytes A contiguous byte container (e.g., std::vector<uint8_t>, std::string) containing UBJSON data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ContiguousByteContainer auto& _bytes) {
  auto result = detail::decode(reinterpret_cast<const char*>(_bytes.data()),
                               _bytes.size());
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(
        r, InputVarType{result.value().get()});
  } else {
    return rfl::error(result.error().what());
  }
}

/// Parses an object from a stream containing UBJSON data.
/// Reads UBJSON binary data from the stream and constructs a C++ object.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing UBJSON binary data
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  auto result = detail::decode(_stream);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(
        r, InputVarType{result.value().get()});
  } else {
    return rfl::error(result.error().what());
  }
}

}  // namespace rfl::ubjson

#endif
