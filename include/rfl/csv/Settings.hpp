#ifndef RFL_CSV_SETTINGS_HPP_
#define RFL_CSV_SETTINGS_HPP_

#include <arrow/csv/api.h>
#include <arrow/io/api.h>

#include "../Settings.hpp"

namespace rfl::csv {

struct Settings {
  /// Maximum number of rows processed at a time.
  /// Data is processed in batches of N rows. This number
  /// can impact performance.
  const int32_t batch_size = 1024;

  /// Field delimiter.
  const char delimiter = ',';

  /// Whether quoting is used.
  const bool quoting = true;

  /// Quoting character (if quoting is true). Only relevant for reading.
  const char quote_char = '"';

  /// The string to be used for null values. Quotes are not allowed in this
  /// string.
  const std::string null_string = "n/a";

  /// Whether a quote inside a value is double-quoted. Only relevant for
  /// reading.
  const bool double_quote = true;

  /// Whether escaping is used. Only relevant for reading.
  const bool escaping = false;

  /// Escaping character (if escaping is true). Only relevant for reading.
  const char escape_char = arrow::csv::kDefaultEscapeChar;

  /// Whether values are allowed to contain CR (0x0d) and LF (0x0a)
  /// characters. Only relevant for reading.
  const bool newlines_in_values = false;

  /// Whether empty lines are ignored.
  /// If false, an empty line represents a single empty value (assuming a
  /// one-column CSV file). Only relevant for reading.
  const bool ignore_empty_lines = true;

  RFL_SETTINGS_OPS(Settings)
};

}  // namespace rfl::csv

#endif
