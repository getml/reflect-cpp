#ifndef GENERIC_READ_HPP_
#define GENERIC_READ_HPP_

#include <istream>
#include <vector>

#include "../Generic.hpp"
#include "../Processors.hpp"
#include "../Result.hpp"
#include "Parser.hpp"

namespace rfl {
namespace generic {

/// Parses an object from a generic type.
template <class T, class... Ps>
auto read(const Generic& _g) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _g);
}

}  // namespace generic
}  // namespace rfl

#endif
