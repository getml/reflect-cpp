#ifndef RFL_CSV_WRITE_HPP_
#define RFL_CSV_WRITE_HPP_

#include <arrow/csv/api.h>
#include <arrow/csv/writer.h>
#include <arrow/io/api.h>

#include <cstdint>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "../Processors.hpp"
#include "../Ref.hpp"
#include "../parsing/tabular/ArrowWriter.hpp"
#include "Settings.hpp"

namespace rfl::csv {

/// Returns CSV bytes.
template <class... Ps>
Ref<arrow::Buffer> to_buffer(const auto& _arr, const Settings& _settings) {
  using T = std::remove_cvref_t<decltype(_arr)>;

  const auto table =
      parsing::tabular::ArrowWriter<T, parsing::tabular::SerializationType::csv,
                                    Ps...>(_settings.batch_size)
          .to_table(_arr);

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

/// Returns CSV bytes.
template <class... Ps>
std::string write(const auto& _arr, const Settings& _settings = Settings{}) {
  const auto buffer = to_buffer<Ps...>(_arr, _settings);
  const auto view = std::string_view(*buffer);
  return std::string(view);
}

/// Writes a CSV into an ostream.
template <class... Ps>
std::ostream& write(const auto& _arr, std::ostream& _stream,
                    const Settings& _settings = Settings{}) {
  auto buffer = to_buffer<Ps...>(_arr, _settings);
  _stream << std::string_view(*buffer);
  return _stream;
}

}  // namespace rfl::csv

#endif
