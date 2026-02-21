#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include <rfl.hpp>
#include <rfl/cli.hpp>

// cli::Reader stoull accepts negative numbers for unsigned types
// File: include/rfl/cli/Reader.hpp:77
// std::stoull("-1") returns ULLONG_MAX (wraps around), then static_cast<T>
// wraps again. No check for negative input.
namespace test_cli_stoull_negative {

struct Config {
  uint16_t port;
};

TEST(regression, cli_rejects_negative_for_unsigned) {
  const char* args[] = {"program", "--port=-1"};
  const auto result = rfl::cli::read<Config>(2, const_cast<char**>(args));
  EXPECT_FALSE(result)
      << "cli::read should reject negative value for uint16_t, "
         "but parsed port=" << result.value().port;
}

}  // namespace test_cli_stoull_negative

// cli::Reader no range check on narrowing cast
// File: include/rfl/cli/Reader.hpp:77
// static_cast<T>(stoull(str)) silently truncates. E.g. stoull("99999") = 99999,
// then static_cast<uint16_t>(99999) = 34463.
namespace test_cli_narrowing_cast {

struct Config {
  uint16_t port;
};

TEST(regression, cli_rejects_out_of_range_for_narrow_type) {
  const char* args[] = {"program", "--port=99999"};
  const auto result = rfl::cli::read<Config>(2, const_cast<char**>(args));
  EXPECT_FALSE(result)
      << "cli::read should reject 99999 for uint16_t (max 65535), "
         "but parsed port=" << result.value().port;
}

}  // namespace test_cli_narrowing_cast
