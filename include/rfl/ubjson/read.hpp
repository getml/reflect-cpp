#ifndef RFL_UBJSON_READ_HPP_
#define RFL_UBJSON_READ_HPP_

#include <bit>
#include <istream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/ubjson/decode_ubjson.hpp>
#include <string>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::ubjson {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from UBJSON using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ContiguousByteContainer auto& _bytes) {
  // TODO: Use a non-throwing decode_ubjson(), pending
  // https://github.com/danielaparker/jsoncons/issues/615
  try {
    auto val = jsoncons::ubjson::decode_ubjson<jsoncons::json>(_bytes);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch (const jsoncons::ser_error& e) {
    std::string error("Could not parse UBJSON: ");
    error.append(e.what());
    return rfl::error(error);
  }
}

/// Parses an object from a stream.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  // TODO: Use a non-throwing decode_ubjson(), pending
  // https://github.com/danielaparker/jsoncons/issues/615
  try {
    auto val = jsoncons::ubjson::decode_ubjson<jsoncons::json>(_stream);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch (const jsoncons::ser_error& e) {
    std::string error("Could not parse UBJSON: ");
    error.append(e.what());
    return rfl::error(error);
  }
}

}  // namespace rfl::ubjson

#endif
