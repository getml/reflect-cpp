#ifndef RFL_PARQUET_READ_HPP_
#define RFL_PARQUET_READ_HPP_

#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>

#include <istream>
#include <string>

#include "../Result.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "../parsing/tabular/ArrowReader.hpp"

namespace rfl::parquet {

/// @brief Parses an object from PARQUET using reflection.
/// @tparam T The type to parse.
/// @tparam Ps Additional parameters for parsing.
/// @param _bytes Pointer to the byte buffer containing the PARQUET data.
/// @param _size Size of the byte buffer.
/// @return Result containing the parsed object or an error message.
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

  const auto table_or = arrow_reader.ValueOrDie()->ReadTable();

  if (!table_or.ok()) {
    return error("Could not read table: " + table_or.status().message());
  }

  auto& table = table_or.ValueOrDie();

  using ArrowReader = parsing::tabular::ArrowReader<
      T, parsing::tabular::SerializationType::parquet, Ps...>;

  return ArrowReader::make(table).and_then(
      [](const auto& _r) { return _r.read(); });
}

/// @brief Parses an object from PARQUET using reflection.
/// @tparam T The type to parse.
/// @tparam Ps Additional parameters for parsing.
/// @param _bytes Contiguous byte container holding the PARQUET data.
/// @return Result containing the parsed object or an error message.
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// @brief Parses an object from a stream.
/// @tparam T The type to parse.
/// @tparam Ps Additional parameters for parsing.
/// @param _stream Input stream containing the PARQUET data.
/// @return Result containing the parsed object or an error message.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::parquet

#endif
