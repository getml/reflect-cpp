#ifndef GENERIC_WRITE_HPP_
#define GENERIC_WRITE_HPP_

#include "../Generic.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace generic {

/// Converts a C++ object to a Generic value using reflection.
/// The Generic type is a format-agnostic intermediate representation that
/// can be easily converted to any serialization format (JSON, YAML, etc.).
/// @tparam Ps Optional processors to apply during serialization
/// @param _t The object to convert
/// @return A Generic value representing the object's structure and data
template <class... Ps>
Generic write(const auto& _t) {
  using T = std::remove_cvref_t<decltype(_t)>;
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer();
  Parser<T, Processors<Ps...>>::write(w, _t, typename ParentType::Root{});
  return w.root();
}

}  // namespace generic
}  // namespace rfl

#endif
