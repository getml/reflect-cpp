#ifndef RFL_TEST_CEREAL_ARCHIVE_REFERENCE_HPP_
#define RFL_TEST_CEREAL_ARCHIVE_REFERENCE_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace cereal {
class PortableBinaryInputArchive;
class PortableBinaryOutputArchive;
}  // namespace cereal

namespace test_cereal_archive {
struct Record {
  std::int16_t small;
  std::uint64_t large;
  double number;
  bool flag;
  std::string text;
};

std::vector<char> reference_bytes(const Record& _value);
Record reference_read(const std::vector<char>& _bytes);
std::vector<char> with_output_archive(
    const std::function<void(::cereal::PortableBinaryOutputArchive&)>& _write);
void with_input_archive(
    const std::vector<char>& _bytes,
    const std::function<void(::cereal::PortableBinaryInputArchive&)>& _read);
}  // namespace test_cereal_archive

#endif
