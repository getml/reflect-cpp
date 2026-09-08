#ifndef RFL_ENV_READ_HPP_
#define RFL_ENV_READ_HPP_

#include "../Processors.hpp"
#include "../ToAllCaps.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::env {

/// This function provides a convenient way to read environment variables into a
/// type T.
/// It automatically applies the ToAllCaps processor to the environment variable
/// keys, allowing the user to specify field names in snake_case or camelCase in
/// their struct, and they will automatically match the corresponding uppercase
/// environment variables. The Processors pack allows for additional custom
/// processing to be applied.
template <class T, class... Ps>
rfl::Result<T> read() {
  using InputVarType = typename Reader::InputVarType;
  const auto r = Reader();
  return Parser<T, Processors<Ps..., ToAllCaps>>::read(r, InputVarType{});
}

}  // namespace rfl::env

#endif
