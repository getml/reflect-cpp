#ifndef RFL_CBOR_READ_HPP_
#define RFL_CBOR_READ_HPP_

#include <bit>
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
  try {
    auto val = jsoncons::cbor::decode_cbor<jsoncons::json>(_bytes);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch (const jsoncons::ser_error& e) {
    std::string error("Could not parse CBOR: ");
    error.append(e.what());
    return rfl::error(error);
  }
}

/// Parses an object from a stream.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  // TODO: Use a non-throwing decode_cbor(), pending
  // https://github.com/danielaparker/jsoncons/issues/615
  try {
    auto val = jsoncons::cbor::decode_cbor<jsoncons::json>(_stream);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch (const jsoncons::ser_error& e) {
    std::string error("Could not parse CBOR: ");
    error.append(e.what());
    return rfl::error(error);
  }
}

}  // namespace rfl::cbor

#endif
