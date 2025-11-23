#ifndef RFL_CBOR_READ_HPP_
#define RFL_CBOR_READ_HPP_

#include <istream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/decode_cbor.hpp>
#include <string>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::cbor {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from CBOR using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ContiguousByteContainer auto& _bytes) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_bytes);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    return rfl::error("Could not parse CBOR: " + result.error().message());
  }
}

/// Parses an object from a stream.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_stream);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    return rfl::error("Could not parse CBOR: " + result.error().message());
  }
}

}  // namespace rfl::cbor

#endif
