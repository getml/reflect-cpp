#ifndef GENERIC_READ_HPP_
#define GENERIC_READ_HPP_


#include "../Generic.hpp"
#include "../Processors.hpp"
#include "Parser.hpp"

namespace rfl {
namespace generic {

/// Parses a C++ object from a Generic value using reflection.
/// This is useful for converting between different serialization formats:
/// format A → Generic → format B, providing format independence.
/// @tparam T The C++ type to deserialize into
/// @tparam Ps Optional processors to apply during deserialization
/// @param _g The Generic value to parse
/// @return A Result containing the deserialized object or an error
template <class T, class... Ps>
auto read(const Generic& _g) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _g);
}

}  // namespace generic
}  // namespace rfl

#endif
