#ifndef RFL_ENV_WRITE_HPP_
#define RFL_ENV_WRITE_HPP_

#include "../Processors.hpp"
#include "../ToAllCaps.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::env {

/// Serializes the given object `_obj` into the environment variables.
///
/// This function acts as a high-level entry point for serialization. It:
///
/// @tparam Ps Variadic list of additional processors to apply during parsing.
/// @param _obj The object to serialize.
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
