#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/cbor.hpp>
#include <string>

// Helper function to parse hex bytes from CBOR diagnostic output
template<typename CharT>
std::vector<CharT> parse_cbor_diagnostic_hex(const std::string& cbor_diagnostic_hex) {
    std::vector<CharT> result;
    std::istringstream iss{cbor_diagnostic_hex};
    std::string token;
    
    while (iss >> token) {
        // Skip comments (lines starting with #)
        if (token.starts_with("#")) {
            // Skip the rest of the line
            std::string line;
            std::getline(iss, line);
            continue;
        }
        
        // Check if token is a valid hex string (even number of hex digits)
        if (token.length() >= 2 && token.length() % 2 == 0 &&
            std::all_of(token.begin(), token.end(), [](char c) {
                return std::isxdigit(c);
            })) {

            // Process hex string in pairs of characters
            for (size_t i = 0; i < token.length(); i += 2) {
                std::string hex_byte = token.substr(i, 2);
                CharT byte = static_cast<CharT>(
                    std::stoul(hex_byte, nullptr, 16)
                );
                result.push_back(byte);
            }
        }
    }
    
    return result;
}

// TODO: Ideally, we'd find a clean way to bundle the https://github.com/BlockchainCommons/bc-dcbor-cli
// utility and use it to diff the annotated hex output, but this will do for now.
template<typename CharT>
std::string print_cbor_me_link( const std::vector<CharT>& buffer )
{
  std::ostringstream oss;
  oss << "decode " << buffer.size() << " CBOR bytes at:\n  https://cbor.me/?bytes=";
  oss << std::setfill('0') << std::hex;
  for ( auto b: buffer ) {
      oss << std::setw(2) << (int32_t)(uint8_t) b;
  }
  oss << "\n";
  return oss.str();
}

template <class... Ps>
void write_and_read(const auto& object) {
  using T = std::remove_cvref_t<decltype(object)>;
  const auto serialized1 = rfl::cbor::write<Ps...>(object);
  const auto res = rfl::cbor::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::cbor::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2)
      << "\nTest failed on write. For first pass, " << print_cbor_me_link(serialized1)
      << "\nFor second pass, " << print_cbor_me_link(serialized2);
}

template <class... Ps>
void write_and_read(const auto& object, const std::string& expected_diagnostic_hex) {
  using T = std::remove_cvref_t<decltype(object)>;
  const auto serialized1 = rfl::cbor::write<Ps...>(object);
  const std::vector<char> expected_cbor = parse_cbor_diagnostic_hex<char>(expected_diagnostic_hex);
  EXPECT_EQ(serialized1, expected_cbor)
      << "\nTest failed on write. For expected, " << print_cbor_me_link(expected_cbor)
      << "\nFor actual, " << print_cbor_me_link(serialized1);
  const auto res = rfl::cbor::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::cbor::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2)
      << "\nTest failed on write. For first pass, " << print_cbor_me_link(serialized1)
      << "\nFor second pass, " << print_cbor_me_link(serialized2);
}

#endif
