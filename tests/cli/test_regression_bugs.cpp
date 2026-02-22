#include <gtest/gtest.h>

#include <clocale>
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

// cli::Reader uses std::stod which is locale-dependent
// File: include/rfl/cli/Reader.hpp:64
// std::stod uses the current C locale. In locales where the decimal
// separator is a comma (e.g. de_DE), "3.14" parses incorrectly.
namespace test_cli_stod_locale {

struct FloatConfig {
  double rate;
};

TEST(regression, cli_float_parsing_ignores_locale) {
  // Save current locale
  const char* old_locale = std::setlocale(LC_NUMERIC, nullptr);
  std::string saved_locale = old_locale ? old_locale : "C";

  // Try to set a locale that uses comma as decimal separator
  const char* result = std::setlocale(LC_NUMERIC, "de_DE.UTF-8");
  if (!result) {
    result = std::setlocale(LC_NUMERIC, "de_DE");
  }
  if (!result) {
    // Locale not available on this system, skip test
    GTEST_SKIP() << "de_DE locale not available, skipping locale test";
  }

  const char* args[] = {"program", "--rate=3.14"};
  const auto res = rfl::cli::read<FloatConfig>(2, const_cast<char**>(args));

  // Restore locale before assertions
  std::setlocale(LC_NUMERIC, saved_locale.c_str());

  ASSERT_TRUE(res) << "cli::read should parse '3.14' even in de_DE locale. "
                       "Error: " << res.error().what();
  EXPECT_DOUBLE_EQ(res.value().rate, 3.14)
      << "cli::read parsed '3.14' as " << res.value().rate
      << " due to locale-dependent std::stod";
}

}  // namespace test_cli_stod_locale
