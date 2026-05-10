#ifndef RFL_CSV_SETTINGS_HPP_
#define RFL_CSV_SETTINGS_HPP_

#include <arrow/csv/api.h>
#include <arrow/io/api.h>

#include "../Settings.hpp"
#include "../internal/deprecated_with.hpp"

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

  RFL_DEPRECATED_WITH(batch_size)
  Settings with_batch_size(const int32_t _batch_size) const noexcept {
    return replace(*this, make_field<"batch_size">(_batch_size));
  }

  RFL_DEPRECATED_WITH(delimiter)
  Settings with_delimiter(const char _delimiter) const noexcept {
    return replace(*this, make_field<"delimiter">(_delimiter));
  }

  RFL_DEPRECATED_WITH(quoting)
  Settings with_quoting(const bool _quoting) const noexcept {
    return replace(*this, make_field<"quoting">(_quoting));
  }

  RFL_DEPRECATED_WITH(quote_char)
  Settings with_quote_char(const char _quote_char) const noexcept {
    return replace(*this, make_field<"quote_char">(_quote_char));
  }

  RFL_DEPRECATED_WITH(null_string)
  Settings with_null_string(const std::string& _null_string) const noexcept {
    return replace(*this, make_field<"null_string">(_null_string));
  }

  RFL_DEPRECATED_WITH(double_quote)
  Settings with_double_quote(const bool _double_quote) const noexcept {
    return replace(*this, make_field<"double_quote">(_double_quote));
  }

  RFL_DEPRECATED_WITH(escaping)
  Settings with_escaping(const bool _escaping) const noexcept {
    return replace(*this, make_field<"escaping">(_escaping));
  }

  RFL_DEPRECATED_WITH(escape_char)
  Settings with_escape_char(const char _escape_char) const noexcept {
    return replace(*this, make_field<"escape_char">(_escape_char));
  }

  RFL_DEPRECATED_WITH(newlines_in_values)
  Settings with_newlines_in_values(
      const bool _newlines_in_values) const noexcept {
    return replace(*this,
                   make_field<"newlines_in_values">(_newlines_in_values));
  }

  RFL_DEPRECATED_WITH(ignore_empty_lines)
  Settings with_ignore_empty_lines(
      const bool _ignore_empty_lines) const noexcept {
    return replace(*this,
                   make_field<"ignore_empty_lines">(_ignore_empty_lines));
  }
};

}  // namespace rfl::csv

#endif
