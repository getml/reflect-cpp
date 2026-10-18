#include "archive_reference.hpp"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>

namespace test_cereal_archive {
std::vector<char> with_output_archive(
    const std::function<void(::cereal::PortableBinaryOutputArchive&)>& _write) {
  std::stringstream stream;
  {
    ::cereal::PortableBinaryOutputArchive archive(stream);
    _write(archive);
  }
  const auto str = stream.str();
  return {str.begin(), str.end()};
}

void with_input_archive(
    const std::vector<char>& _bytes,
    const std::function<void(::cereal::PortableBinaryInputArchive&)>& _read) {
  std::stringstream stream(std::string(_bytes.begin(), _bytes.end()));
  ::cereal::PortableBinaryInputArchive archive(stream);
  _read(archive);
}

std::vector<char> reference_bytes(const Record& _value) {
  return with_output_archive([&](auto& archive) {
    archive(_value.small, _value.large, _value.number, _value.flag,
            _value.text);
  });
}

Record reference_read(const std::vector<char>& _bytes) {
  Record value;
  with_input_archive(_bytes, [&](auto& archive) {
    archive(value.small, value.large, value.number, value.flag, value.text);
  });
  return value;
}
}  // namespace test_cereal_archive
