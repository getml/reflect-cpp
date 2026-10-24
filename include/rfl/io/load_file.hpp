#ifndef RFL_IO_LOAD_FILE_HPP_
#define RFL_IO_LOAD_FILE_HPP_

#include <array>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <system_error>

#include "../Result.hpp"

namespace rfl::io::internal {

template <class Container>
Result<Container> load_file(const std::string& _fname,
                            const std::ios::openmode _mode) {
  try {
    std::ifstream input(_fname, _mode);
    if (!input.is_open()) {
      return error("Unable to open file '" + _fname + "' for reading.");
    }
    Container bytes;
    std::error_code ec;
    const auto size = std::filesystem::file_size(_fname, ec);
    if (!ec) {
      if (size > bytes.max_size() ||
          size > static_cast<std::uintmax_t>(
                     (std::numeric_limits<std::streamsize>::max)())) {
        return error("File '" + _fname + "' is too large to read.");
      }
      bytes.resize(static_cast<std::size_t>(size));
      if (!bytes.empty()) {
        input.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        bytes.resize(static_cast<std::size_t>(input.gcount()));
        if (input.bad() || (input.fail() && !input.eof())) {
          return error("Unable to read file '" + _fname + "'.");
        }
        if (input.eof()) {
          return bytes;
        }
      }
    } else if (std::filesystem::is_directory(_fname, ec)) {
      return error("Cannot read directory '" + _fname + "' as a file.");
    }
    // The size is only a hint: text conversion, growing files, and special
    // files can produce a different number of bytes. Never require seeking.
    std::array<char, 64 * 1024> buffer;
    do {
      input.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
      if (input.bad() || (input.fail() && !input.eof())) {
        return error("Unable to read file '" + _fname + "'.");
      }
      bytes.insert(bytes.end(), buffer.data(), buffer.data() + input.gcount());
    } while (!input.eof());
    return bytes;
  } catch (const std::exception& e) {
    return error("Unable to read file '" + _fname + "': " + e.what());
  }
}

}  // namespace rfl::io::internal

#endif
