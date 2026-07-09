#ifndef RFL_ENV_WRITE_HPP_
#define RFL_ENV_WRITE_HPP_

#include "../Processors.hpp"
#include "../ToAllCaps.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::env {

template <class... Ps>
void write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer();
  Parser<T, Processors<Ps..., ToAllCaps>>::write(w, _obj,
                                                 typename ParentType::Root{});
}

}  // namespace rfl::env

#endif
