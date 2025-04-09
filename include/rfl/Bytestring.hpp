#ifndef RFL_BYTESTRING_HPP_
#define RFL_BYTESTRING_HPP_

#include <cstddef>
#include <vector>

namespace rfl {
// custom type to avoid serializing this as a vector of enums
// in other means this is the same as
// using Bytestring = std::vector<std::byte>;
class Bytestring : public std::vector<std::byte> {
public:
  using std::vector<std::byte>::vector;
};

}  // namespace rfl

#endif
