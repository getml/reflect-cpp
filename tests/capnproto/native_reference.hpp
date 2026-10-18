#ifndef RFL_TEST_CAPNPROTO_NATIVE_REFERENCE_HPP_
#define RFL_TEST_CAPNPROTO_NATIVE_REFERENCE_HPP_
#include <cstdint>
#include <string>
#include <vector>
namespace capnp {
class ParsedSchema;
}
namespace test_capnproto_bridge {
struct Record {
  std::string text;
  int64_t count;
  bool ready;
  std::vector<double> values;
};
std::vector<char> reference_bytes(const capnp::ParsedSchema&, const Record&);
Record reference_read(const capnp::ParsedSchema&, const std::vector<char>&);
}  // namespace test_capnproto_bridge
#endif
