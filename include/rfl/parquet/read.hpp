#ifndef RFL_PARQUET_READ_HPP_
#define RFL_PARQUET_READ_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>

#include <istream>
#include <memory>
#include <string>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "../parsing/tabular/ArrowReader.hpp"

namespace rfl::parquet {

/// Parses an object from PARQUET using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  arrow::MemoryPool* pool = arrow::default_memory_pool();

  const auto buffer = std::make_shared<arrow::Buffer>(
      internal::ptr_cast<const uint8_t*>(_bytes), _size);

  const auto input = std::make_shared<arrow::io::BufferReader>(buffer);

  auto arrow_reader = ::parquet::arrow::OpenFile(input, pool);

  if (!arrow_reader.ok()) {
    return error(std::string("Could not generate the arrow reader: ") +
                 arrow_reader.status().message());
  }

  std::shared_ptr<arrow::Table> table;

  const auto status = arrow_reader.ValueOrDie()->ReadTable(&table);

  if (!status.ok()) {
    return error("Could not read table: " + status.message());
  }

  using ArrowReader = parsing::tabular::ArrowReader<
      T, parsing::tabular::SerializationType::parquet, Ps...>;

  return ArrowReader::make(table).and_then(
      [](const auto& _r) { return _r.read(); });
}

/// Parses an object from PARQUET using reflection.
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::parquet

#endif
