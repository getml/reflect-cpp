#include <arrow/csv/api.h>
#include <arrow/io/api.h>

#include "rfl/csv/Table.hpp"

namespace rfl::csv::detail {
Result<std::shared_ptr<arrow::Table>> read_table(const char* _str,
                                                 const size_t _size,
                                                 const Settings& _settings) {
  arrow::io::IOContext io_context = arrow::io::default_io_context();

  const auto buffer = std::make_shared<arrow::Buffer>(
      internal::ptr_cast<const uint8_t*>(_str), _size);

  std::shared_ptr<arrow::io::InputStream> input =
      std::make_shared<arrow::io::BufferReader>(buffer);

  auto read_options = arrow::csv::ReadOptions::Defaults();
  auto convert_options = arrow::csv::ConvertOptions::Defaults();
  convert_options.null_values =
      std::vector<std::string>({_settings.null_string});
  convert_options.strings_can_be_null = true;

  auto parse_options = arrow::csv::ParseOptions::Defaults();
  parse_options.delimiter = _settings.delimiter;
  parse_options.quoting = _settings.quoting;
  parse_options.quote_char = _settings.quote_char;
  parse_options.double_quote = _settings.double_quote;
  parse_options.escaping = _settings.escaping;
  parse_options.escape_char = _settings.escape_char;
  parse_options.newlines_in_values = _settings.newlines_in_values;
  parse_options.ignore_empty_lines = _settings.ignore_empty_lines;

  auto maybe_reader = arrow::csv::TableReader::Make(
      io_context, input, read_options, parse_options, convert_options);

  if (!maybe_reader.ok()) {
    return error("Could not construct CSV reader: " +
                 maybe_reader.status().message());
  }

  std::shared_ptr<arrow::csv::TableReader> reader = *maybe_reader;

  auto maybe_table = reader->Read();
  if (!maybe_table.ok()) {
    return error("Could not read table: " + maybe_table.status().message());
  }

  const std::shared_ptr<arrow::Table> table = *maybe_table;

  return table;
}
Ref<arrow::Buffer> write_table(const std::shared_ptr<arrow::Table>& table,
                               const Settings& _settings) {
  const auto output_buffer = arrow::io::BufferOutputStream::Create();

  if (!output_buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  auto options = arrow::csv::WriteOptions::Defaults();
  options.batch_size = _settings.batch_size;
  options.delimiter = _settings.delimiter;
  options.null_string = _settings.null_string;
  options.quoting_style = _settings.quoting ? arrow::csv::QuotingStyle::Needed
                                            : arrow::csv::QuotingStyle::None;

  const auto status =
      arrow::csv::WriteCSV(*table, options, output_buffer.ValueOrDie().get());

  if (!status.ok()) {
    throw std::runtime_error(status.message());
  }

  const auto buffer = output_buffer.ValueOrDie()->Finish();

  if (!buffer.ok()) {
    throw std::runtime_error(output_buffer.status().message());
  }

  return Ref<arrow::Buffer>::make(buffer.ValueOrDie()).value();
}
}  // namespace rfl::csv::detail
