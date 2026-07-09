#ifndef RFL_ENV_READ_HPP_
#define RFL_ENV_READ_HPP_

#include "../Processors.hpp"
#include "../ToAllCaps.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::env {

template <class T, class... Ps>
rfl::Result<T> read() {
  using InputVarType = typename Reader::InputVarType;
  const auto r = Reader();
  return Parser<T, Processors<Ps..., ToAllCaps>>::read(r, InputVarType{});
}

}  // namespace rfl::env

#endif
