#ifndef GENERIC_WRITE_HPP_
#define GENERIC_WRITE_HPP_

#include <cstddef>
#include <ostream>
#include <sstream>
#include <vector>

#include "../Generic.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace generic {

/// Writes an object to a generic.
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
